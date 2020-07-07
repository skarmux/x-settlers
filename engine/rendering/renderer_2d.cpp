#include "rendering/renderer_2d.h"
#include "rendering/render_command.h"
#include "rendering/shader.h"
#include "rendering/vertex_array.h"
#include "rendering/buffer.h"

struct MapVertex
{
	glm::vec2 pos;
	float shade = 1.0f;
};

struct QuadVertex
{
	glm::vec2 pos;
	glm::vec2 tex;
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
	QuadVertex* quad_vertex_buffer_base = nullptr;
	QuadVertex* quad_vertex_buffer_ptr = nullptr;

	glm::vec2 quad_vertex_positions[4];

	std::shared_ptr<Shader> quad_shader;

	uint32_t quad_index_count = 0;

	// MAP
	static const uint32_t MAX_MAP_SIZE = 448;
	static const uint32_t MAX_MAP_VERTICES = MAX_MAP_SIZE * MAX_MAP_SIZE;
	static const uint32_t MAX_MAP_TRIANGLES = (MAX_MAP_SIZE - 1) * (MAX_MAP_SIZE - 1) * 2;

	std::vector<std::shared_ptr<VertexArray>> map_vertex_arrays;
	std::vector<std::shared_ptr<IndexBuffer>> map_index_buffers;
	std::vector<std::shared_ptr<VertexBuffer>> map_tex_coord_buffers;
	std::shared_ptr<VertexBuffer> map_vertex_buffer;

	std::shared_ptr<Shader> map_shader;

	std::vector<uint32_t> map_index_counts;
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

	s_data.quad_vertex_buffer_base = new QuadVertex[s_data.MAX_QUAD_VERTICES];

	// fill quad indices buffer
	uint32_t* quad_indices = new uint32_t[s_data.MAX_QUAD_INDICES];
	uint32_t offset = 0;
	for (uint32_t i = 0; i < s_data.MAX_QUAD_INDICES; i += 6)
	{
		quad_indices[i + 0] = offset + 0;
		quad_indices[i + 1] = offset + 1;
		quad_indices[i + 2] = offset + 2;

		quad_indices[i + 3] = offset + 2;
		quad_indices[i + 4] = offset + 3;
		quad_indices[i + 5] = offset + 0;

		offset += 4;
	}
	s_data.quad_index_buffer = IndexBuffer::create(quad_indices, s_data.MAX_QUAD_INDICES);
	s_data.quad_vertex_array->set_index_buffer(s_data.quad_index_buffer);
	delete[] quad_indices;

	// create quad shader
	s_data.quad_shader = Shader::create("assets/shaders/opengl/quad.glsl");

	// set quad edges offset to quad position
	// the quads origin position lays centered on its bottom line
	s_data.quad_vertex_positions[0] = { -0.5f, 0.0f }; // bottom left
	s_data.quad_vertex_positions[1] = { 0.5f, 0.0f }; // bottom right
	s_data.quad_vertex_positions[2] = { 0.5f, 1.0f }; // top right
	s_data.quad_vertex_positions[3] = { -0.5f, 1.0f }; // top left

	// ===========================================================================

	// MAP
	s_data.map_vertex_buffer = VertexBuffer::create(s_data.MAX_MAP_VERTICES * sizeof(MapVertex));
	s_data.map_vertex_buffer->set_layout({
		{ ShaderDataType::Float2, "a_Position" },
		{ ShaderDataType::Float1, "a_Shade" } });

	s_data.map_vertex_arrays.resize(6);
	s_data.map_tex_coord_buffers.resize(6);
	for (int i = 0; i < 6; i++)
	{
		s_data.map_vertex_arrays[i] = VertexArray::create();
		s_data.map_tex_coord_buffers[i] = VertexBuffer::create(s_data.MAX_MAP_VERTICES * sizeof(glm::vec2));
		s_data.map_tex_coord_buffers[i]->set_layout({
			{ ShaderDataType::Float2, "a_TexCoord" } });
		s_data.map_vertex_arrays[i]->add_vertex_buffer(s_data.map_vertex_buffer);
		s_data.map_vertex_arrays[i]->add_vertex_buffer(s_data.map_tex_coord_buffers[i]);
	}

	// fill map triangle indices buffer
	std::vector<std::vector<uint32_t>> map_indices(6);

	//    (0)--- 1
	//    / \ B /  A : 0 > 3 > 2 (clockwise)
	//   / A \ /   B : 0 > 1 > 3 (clockwise)
	//  2 --- 3

	int index_buffer_id = 0; // [0-6]

	uint32_t index_0, index_1, index_2, index_3;

	for (int row = 0; row < (s_data.MAX_MAP_SIZE - 1); row++)
	{
		for (int col = 0; col < (s_data.MAX_MAP_SIZE - 1); col++)
		{
			index_0 = (s_data.MAX_MAP_SIZE * row) + col;
			index_1 = index_0 + 1;
			index_2 = index_0 + s_data.MAX_MAP_SIZE;
			index_3 = index_0 + s_data.MAX_MAP_SIZE + 1;

			index_buffer_id = ((col * 2) + (row * 2)) % 6;

			switch (index_buffer_id)
			{
			case 0:
				map_indices[0].push_back(index_0); // A
				map_indices[0].push_back(index_3);
				map_indices[0].push_back(index_2);
				map_indices[1].push_back(index_0); // B
				map_indices[1].push_back(index_1);
				map_indices[1].push_back(index_3);
				break;
			case 2:
				map_indices[2].push_back(index_0); // A
				map_indices[2].push_back(index_3);
				map_indices[2].push_back(index_2);
				map_indices[3].push_back(index_0); // B
				map_indices[3].push_back(index_1);
				map_indices[3].push_back(index_3);
				break;
			case 4:
				map_indices[4].push_back(index_0); // A
				map_indices[4].push_back(index_3);
				map_indices[4].push_back(index_2);
				map_indices[5].push_back(index_0); // B
				map_indices[5].push_back(index_1);
				map_indices[5].push_back(index_3);
				break;
			default:
				LOGIC_ERROR("tex_id got a wrong value: {0}", index_buffer_id);
			}
		}
	}
	
	s_data.map_index_buffers.resize(6);
	for (int i = 0; i < 6; i++)
	{
		s_data.map_index_buffers[i] = IndexBuffer::create(map_indices[i].data(), map_indices[i].size());
		s_data.map_vertex_arrays[i]->set_index_buffer(s_data.map_index_buffers[i]);
		map_indices[i].clear();
	}
	map_indices.clear();

	s_data.map_index_counts.resize(6);
	s_data.map_index_counts[0] = 0;

	// create map shader
	s_data.map_shader = Shader::create("assets/shaders/opengl/map.glsl");
}

void Renderer2D::shutdown()
{
	delete[] s_data.quad_vertex_buffer_base;
}

void Renderer2D::begin_scene(const OrthographicCamera& camera)
{
	s_data.quad_shader->bind();
	s_data.quad_shader->set_mat4("u_ViewProjection", camera.get_view_projection_matrix());
	
	s_data.map_shader->bind();
	s_data.map_shader->set_mat4("u_ViewProjection", camera.get_view_projection_matrix());

	s_data.quad_index_count = 0;
	s_data.quad_vertex_buffer_ptr = s_data.quad_vertex_buffer_base;
}

void Renderer2D::end_scene()
{
	// declare size/amount of data within quad_vertex_buffer
	uint32_t dataSize = (uint32_t)( (uint8_t*)s_data.quad_vertex_buffer_ptr - (uint8_t*)s_data.quad_vertex_buffer_base );
	s_data.quad_vertex_buffer->set_data(s_data.quad_vertex_buffer_base, dataSize);

	flush();
}

void Renderer2D::flush()
{
	if (s_data.map_index_counts[0] == 0 && s_data.quad_index_count == 0)
	{
		RENDERER_WARN("Nothing to draw.");
		return;
	}
	
	s_data.map_shader->bind();
	//RenderCommand::draw_indexed(s_data.map_vertex_arrays[0], s_data.map_index_counts[0]);
	for (int i = 0; i < 6; i++)
	{
		RenderCommand::draw_indexed(s_data.map_vertex_arrays[i], s_data.map_index_counts[i]);
	}

	s_data.quad_shader->bind();
	RenderCommand::draw_indexed(s_data.quad_vertex_array, s_data.quad_index_count);
}

void Renderer2D::flush_and_reset()
{
	RENDERER_INFO("Flush and Reset");
	end_scene();

	s_data.quad_index_count = 0;
	s_data.quad_vertex_buffer_ptr = s_data.quad_vertex_buffer_base;
}

void Renderer2D::set_map(const uint32_t& size, const std::vector<uint8_t>& heights, const std::vector<uint8_t>& types)
{
	if (size > s_data.MAX_MAP_SIZE)
		return;
	
	// distance between map vertices
	constexpr uint8_t X_STEP = 16;
	constexpr uint8_t Y_STEP = 9;

	constexpr float TEX_X_RANGE = 8 * X_STEP;
	constexpr float TEX_Y_RANGE = 8 * Y_STEP;
	
	const uint32_t vertex_count = size * size;
	const uint32_t triangle_count = (size - 1) * (size - 1) * 2;

	MapVertex* vertices = new MapVertex[s_data.MAX_MAP_VERTICES];
	MapVertex* vertices_ptr = vertices;

	uint8_t* heights_ptr = (uint8_t*)heights.data();

	glm::vec2** tex_coords = new glm::vec2*[6];
	glm::vec2** tex_coords_ptr = new glm::vec2 * [6];
	for (int i = 0; i < 6; i++)
	{
		tex_coords[i] = new glm::vec2[s_data.MAX_MAP_VERTICES];
		tex_coords_ptr[i] = tex_coords[i];
	}

	for (uint32_t row = 0; row < size; row++)
	{
		/*vertices_ptr = vertices + (row * s_data.MAX_MAP_SIZE);
		for (int j = 0; j < 6; j++)
			tex_coords_ptr[j] = tex_coords[j] + (row * s_data.MAX_MAP_SIZE);*/

		float x_shift = (size - row) * (X_STEP / 2);
		float y = (size - row) * Y_STEP;

		for (uint32_t col = 0; col < size; col++)
		{
			const uint8_t& height = *heights_ptr++;
			vertices_ptr->pos.x = (col * X_STEP) + x_shift;
			vertices_ptr->pos.y = y + height;
			
			if (row < (size - 1))
			{
				const uint8_t& bot_height = heights[((row+1)*size)+col];
				vertices_ptr->shade = 0.875f + ((float)height - (float)bot_height) * 0.0625f;
			}

			for (int j = 0; j < 6; j++)
			{
				tex_coords_ptr[j]->x = vertices_ptr->pos.x / TEX_X_RANGE;
				tex_coords_ptr[j]->y = y / TEX_Y_RANGE;
				tex_coords_ptr[j]++;
			}

			vertices_ptr++;
		}
	}

	for (int i = 0; i < 6; i++)
		s_data.map_index_counts[i] = triangle_count / 6 * 3;

	for (int i = 0; i < triangle_count % 6; i++)
		s_data.map_index_counts[i] += 3;

	s_data.map_vertex_buffer->set_data(vertices, vertex_count * sizeof(MapVertex));
	delete[] vertices;

	for (int i = 0; i < 6; i++)
	{
		s_data.map_tex_coord_buffers[i]->set_data(tex_coords[i], vertex_count * sizeof(glm::vec2));
		delete[] tex_coords[i];
	}
}

void Renderer2D::remove_map()
{
	for (int i = 0; i < 6; i++)
		s_data.map_index_counts[i] = 0;
}

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
