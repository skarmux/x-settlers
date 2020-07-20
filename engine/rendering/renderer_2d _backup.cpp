#include "rendering/renderer_2d.h"
#include "rendering/render_command.h"
#include "rendering/shader.h"
#include "rendering/vertex_array.h"
#include "rendering/buffer.h"

struct MeshVertex
{
	glm::vec2 pos;
	float shade;
	//glm::vec2 tex[6];
};

struct QuadVertex
{
	glm::vec2 pos;
	glm::vec2 tex;
};

struct Mesh
{
	const uint32_t id;
	uint32_t col_count;
	uint32_t row_count;
	MeshVertex* data;
	std::shared_ptr<VertexBuffer> vertex_buffer;
	std::vector<std::shared_ptr<IndexBuffer>> index_buffers;
	std::vector<std::shared_ptr<VertexArray>> vertex_arrays;
};

struct Renderer2DData
{
	// QUADS
	static const uint32_t MAX_QUADS = 20000;
	static const uint32_t MAX_QUAD_VERTICES = MAX_QUADS * 4;
	static const uint32_t MAX_QUAD_INDICES = MAX_QUADS * 6;

	std::shared_ptr<VertexArray> quad_vertex_array;
	std::shared_ptr<IndexBuffer> quad_index_buffer;

	std::shared_ptr<VertexBuffer> quad_vertex_buffer;
	QuadVertex* QUAD_VERTICES;
	QuadVertex* quad_vertex_buffer_ptr = nullptr;

	glm::vec2 quad_vertex_positions[4];

	std::shared_ptr<Shader> quad_shader;

	uint32_t quad_index_count = 0;

	// MESHES
	std::shared_ptr<Shader> mesh_shader;
	std::vector<Mesh> static_meshes;
	uint32_t mesh_id_count = 0;
};

static Renderer2DData s_data;

void Renderer2D::init()
{
	// QUADS
	s_data.quad_vertex_array = VertexArray::create();
	s_data.quad_vertex_buffer = VertexBuffer::create(s_data.MAX_QUAD_VERTICES * sizeof(QuadVertex));
	s_data.quad_vertex_buffer->set_layout({
		{ ShaderDataType::Float2, "a_Position" },
		{ ShaderDataType::Float2, "a_TexCoord" } });
	s_data.quad_vertex_array->add_vertex_buffer(s_data.quad_vertex_buffer);

	s_data.QUAD_VERTICES = new QuadVertex[s_data.MAX_QUAD_VERTICES];

	// fill quad indices buffer
	uint32_t* const QUAD_INDICES = new uint32_t[s_data.MAX_QUAD_INDICES];
	uint32_t offset = 0;
	for (uint32_t i = 0; i < s_data.MAX_QUAD_INDICES; i += 6)
	{
		QUAD_INDICES[i + 0] = offset + 0;
		QUAD_INDICES[i + 1] = offset + 1;
		QUAD_INDICES[i + 2] = offset + 2;

		QUAD_INDICES[i + 3] = offset + 2;
		QUAD_INDICES[i + 4] = offset + 3;
		QUAD_INDICES[i + 5] = offset + 0;

		offset += 4;
	}
	s_data.quad_index_buffer = IndexBuffer::create(QUAD_INDICES, s_data.MAX_QUAD_INDICES);
	s_data.quad_vertex_array->set_index_buffer(s_data.quad_index_buffer);
	delete[] QUAD_INDICES;

	// create quad shader
	s_data.quad_shader = Shader::create("assets/shaders/opengl/quad.glsl");

	// set quad edges offset to quad position
	// the quads origin position lays centered on its bottom line
	s_data.quad_vertex_positions[0] = { -0.5f, 0.0f }; // bottom left
	s_data.quad_vertex_positions[1] = {  0.5f, 0.0f }; // bottom right
	s_data.quad_vertex_positions[2] = {  0.5f, 1.0f }; // top right
	s_data.quad_vertex_positions[3] = { -0.5f, 1.0f }; // top left
}

void Renderer2D::shutdown()
{
	delete[] s_data.QUAD_VERTICES;
}

void Renderer2D::begin_scene(const OrthographicCamera& camera)
{
	s_data.quad_shader->bind();
	s_data.quad_shader->set_mat4("u_ViewProjection", camera.get_view_projection_matrix());
	
	s_data.mesh_shader->bind();
	s_data.mesh_shader->set_mat4("u_ViewProjection", camera.get_view_projection_matrix());

	s_data.quad_index_count = 0;
	s_data.quad_vertex_buffer_ptr = s_data.QUAD_VERTICES;
}

void Renderer2D::end_scene()
{
	// declare size/amount of data within quad_vertex_buffer
	uint32_t dataSize = (uint32_t)( (uint8_t*)s_data.quad_vertex_buffer_ptr - (uint8_t*)s_data.QUAD_VERTICES );
	s_data.quad_vertex_buffer->set_data(s_data.QUAD_VERTICES, dataSize);

	flush();
}

void Renderer2D::flush()
{
	if (s_data.quad_index_count == 0 && s_data.static_meshes.size() == 0)
	{
		RENDERER_WARN("Nothing to draw.");
		return;
	}
	
	s_data.mesh_shader->bind();
	for (Mesh m : s_data.static_meshes)
	{
		for (uint32_t i = 0; i < m.vertex_arrays.size(); i++)
		{
			RenderCommand::draw_indexed(m.vertex_arrays[i], m.index_buffers[i]->get_count());
		}
	}

	s_data.quad_shader->bind();
	RenderCommand::draw_indexed(s_data.quad_vertex_array, s_data.quad_index_count);
}

void Renderer2D::flush_and_reset()
{
	RENDERER_INFO("Flush and Reset");
	end_scene();

	s_data.quad_index_count = 0;
	s_data.quad_vertex_buffer_ptr = s_data.QUAD_VERTICES;
}



void Renderer2D::set_static_rhomboid_mesh(const uint32_t size, const uint32_t x_step, const uint32_t y_step, uint32_t& mesh_id)
{
	MeshVertex* const VERTICES = new MeshVertex[size * size];
	MeshVertex* vertices_ptr = VERTICES;

	constexpr uint32_t TRIS_PER_VERTEX = 6;

	std::vector<std::vector<uint32_t>> indices(TRIS_PER_VERTEX);

	uint32_t bi = 0; // [0-TRIS_PER_VERTEX]

	for (uint32_t row = 0; row < size; row++)
	{
		for (uint32_t col = 0; col < size; col++)
		{
			vertices_ptr->pos.x = (col * x_step) + ((size - row) * x_step / 2);
			vertices_ptr->pos.y = (size - row) * y_step;
			vertices_ptr->shade = 1.0f;
			vertices_ptr++;

			if (row == (size - 1) || col == (size - 1))
				continue;

			//   (0)--- 1
			//   / \ B /
			//  / A \ /
			// 2 --- 3

			indices[bi].insert(indices[bi].end(), {
				(size * row) + col,				// index 0
				(size * row) + col + size + 1,  // index 3
				(size * row) + col + size		// index 2
				}); // A: 0 > 3 > 2 (clockwise)

			indices[bi + 1].insert(indices[bi + 1].end(), {
				(size * row) + col,				// index 0
				(size * row) + col + 1,			// index 1
				(size * row) + col + size + 1	// index 3
				}); // B: 0 > 1 > 3 (clockwise)

			bi = (bi + 2) % TRIS_PER_VERTEX;
		}

		bi = (row * 2) % TRIS_PER_VERTEX;
	}

	std::vector<std::shared_ptr<VertexArray>> vertex_arrays(TRIS_PER_VERTEX);

	auto vertex_buffer = VertexBuffer::create(size * size * sizeof(MeshVertex));
	vertex_buffer->set_layout({
		{ ShaderDataType::Float2, "a_Position" },
		{ ShaderDataType::Float1, "a_Shade" } });
	vertex_buffer->set_data(VERTICES, size * size * sizeof(MeshVertex));

	std::vector<std::shared_ptr<IndexBuffer>> index_buffers(TRIS_PER_VERTEX);
	for (int i = 0; i < TRIS_PER_VERTEX; i++)
	{
		vertex_arrays[i] = VertexArray::create();
		vertex_arrays[i]->add_vertex_buffer(vertex_buffer);

		index_buffers[i] = IndexBuffer::create(indices[i].data(), indices[i].size());
		vertex_arrays[i]->set_index_buffer(index_buffers[i]);
		indices[i].clear();
	}
	indices.clear();

	s_data.static_meshes.emplace_back(Mesh{s_data.mesh_id_count, size, size, VERTICES, vertex_buffer, index_buffers});

	mesh_id = s_data.mesh_id_count;
	s_data.mesh_id_count++;
}

void Renderer2D::draw_static_mesh(const uint32_t mesh_id)
{

}

void Renderer2D::unset_static_mesh(uint32_t mesh_id)
{
	for (uint32_t i = 0; i < s_data.static_meshes.size(); i++)
	{
		Mesh& mesh = s_data.static_meshes[i];
		if (mesh.id == mesh_id)
		{
			delete[] mesh.data;
			// TODO: free buffer memory on GPU
			s_data.static_meshes.erase(s_data.static_meshes.begin() + i);
			return;
		}
	}
	RENDERER_WARN("mesh with id {0} couldn't be removed because it wasn't found.");
	return;
}

void Renderer2D::update_static_mesh_vertex(const uint32_t vertex_index, const uint32_t mesh_id, const int32_t x = 0, const int32_t y = 0)
{

}

void Renderer2D::update_static_mesh_triangle(const uint32_t triangle_index, const uint32_t mesh_id, const glm::vec2 uv[3])
{

}

//void Renderer2D::set_map(const uint32_t& size, const std::vector<uint8_t>& heights, const std::vector<std::vector<glm::vec2>>& tex_coords)
//{
//	if (size > s_data.MAX_MAP_SIZE)
//		return;
//	
//	// distance between map vertices
//	constexpr uint8_t X_STEP = 16;
//	constexpr uint8_t Y_STEP = 9;
//
//	/*constexpr float TEX_X_RANGE = 8 * X_STEP;
//	constexpr float TEX_Y_RANGE = 8 * Y_STEP;*/
//	
//	const uint32_t vertex_count = size * size;
//	const uint32_t triangle_count = (size - 1) * (size - 1) * 2;
//
//	MeshVertex* vertices = new MeshVertex[s_data.MAX_MAP_VERTICES];
//	MeshVertex* vertices_ptr = vertices;
//
//	uint8_t* heights_ptr = (uint8_t*)heights.data();
//
//	/*glm::vec2** tex_coords = new glm::vec2*[6];
//	glm::vec2** tex_coords_ptr = new glm::vec2 * [6];
//	for (int i = 0; i < 6; i++)
//	{
//		tex_coords[i] = new glm::vec2[s_data.MAX_MAP_VERTICES];
//		tex_coords_ptr[i] = tex_coords[i];
//	}*/
//
//	for (uint32_t row = 0; row < size; row++)
//	{
//		/*vertices_ptr = vertices + (row * s_data.MAX_MAP_SIZE);
//		for (int j = 0; j < 6; j++)
//			tex_coords_ptr[j] = tex_coords[j] + (row * s_data.MAX_MAP_SIZE);*/
//
//		float x_shift = (size - row) * (X_STEP / 2);
//		float y = (size - row) * Y_STEP;
//
//		for (uint32_t col = 0; col < size; col++)
//		{
//			const uint8_t& height = *heights_ptr++;
//			vertices_ptr->pos.x = (col * X_STEP) + x_shift;
//			vertices_ptr->pos.y = y + height;
//			
//			if (row < (size - 1))
//			{
//				const uint8_t& bot_height = heights[((row+1)*size)+col];
//				vertices_ptr->shade = 0.875f + ((float)height - (float)bot_height) * 0.0625f;
//			}
//
//			/*for (int j = 0; j < 6; j++)
//			{
//				tex_coords_ptr[j]->x = vertices_ptr->pos.x / TEX_X_RANGE;
//				tex_coords_ptr[j]->y = y / TEX_Y_RANGE;
//				tex_coords_ptr[j]++;
//			}*/
//
//			vertices_ptr++;
//		}
//	}
//
//	for (int i = 0; i < 6; i++)
//		s_data.map_index_counts[i] = triangle_count / 6 * 3;
//
//	for (int i = 0; i < triangle_count % 6; i++)
//		s_data.map_index_counts[i] += 3;
//
//	s_data.map_vertex_buffer->set_data(vertices, vertex_count * sizeof(MeshVertex));
//	delete[] vertices;
//
//	/*for (int i = 0; i < 6; i++)
//	{
//		s_data.map_tex_coord_buffers[i]->set_data(tex_coords[i], vertex_count * sizeof(glm::vec2));
//		delete[] tex_coords[i];
//	}*/
//}
//
//void Renderer2D::remove_map()
//{
//	for (int i = 0; i < 6; i++)
//		s_data.map_index_counts[i] = 0;
//}

void Renderer2D::draw_quad(const glm::vec2& position, const std::shared_ptr<Texture2D>& texture)
{
	constexpr size_t quad_index_count = 4;
	constexpr glm::vec2 texture_coords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	
	if (s_data.quad_index_count >= Renderer2DData::MAX_QUAD_INDICES) {
		RENDERER_INFO("Buffer size exceeded. Flushing before drawing next quad.");
		flush_and_reset();
	}

	glm::vec2 texture_size((float)texture->get_width(), (float)texture->get_height());

	for (int i = 0; i < quad_index_count; i++) {
		s_data.quad_vertex_buffer_ptr->pos = position + (s_data.quad_vertex_positions[i] * texture_size); // vec2
		s_data.quad_vertex_buffer_ptr->tex = texture_coords[i]; // vec2
		s_data.quad_vertex_buffer_ptr++;
	}

	s_data.quad_index_count += 6;
}

//void Renderer2D::submit(const std::shared_ptr<VertexArray>& vertex_array, uint32_t triangle_vertex_count)
//{
//	s_data.map_vertex_array = vertex_array;
//	s_data.map_vertex_count = triangle_vertex_count;
//}

void Renderer2D::on_window_resize(uint32_t width, uint32_t height)
{
	RenderCommand::set_viewport(0, 0, width, height);
}
