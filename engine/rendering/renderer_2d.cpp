#include "rendering/renderer_2d.h"
#include "rendering/render_command.h"
#include "rendering/shader.h"
#include "rendering/vertex_array.h"
#include "rendering/buffer.h"

struct QuadVertex
{
	glm::vec2 pos;
	glm::vec2 tex;
	int tex_index;
};

struct Renderer2DData
{
	static const uint32_t MAX_QUADS = 2;
	static const uint32_t MAX_QUAD_VERTICES = MAX_QUADS * 4;
	static const uint32_t MAX_QUAD_INDICES = MAX_QUADS * 6;

	static const uint32_t MAX_TEXTURE_SLOTS = 32;
	std::array<std::shared_ptr<Texture2D>, MAX_TEXTURE_SLOTS> texture_slots;
	uint32_t texture_slot_index = 0;

	std::shared_ptr<Shader> tilemap_shader;
	std::shared_ptr<Shader> quad_shader;

	std::shared_ptr<VertexArray> quad_vertex_array;
	std::shared_ptr<IndexBuffer> quad_index_buffer;
	std::shared_ptr<VertexBuffer> quad_vertex_buffer;

	QuadVertex* QUAD_VERTICES;
	QuadVertex* quad_vertex_buffer_ptr = nullptr;

	uint32_t quad_index_count  = 0;
};

static Renderer2DData s_data;

void Renderer2D::init()
{
	s_data.tilemap_shader = Shader::create("assets/shader/opengl/triangle_tilemap.glsl");
	s_data.tilemap_shader->set_int("u_Atlas", 0);
	s_data.tilemap_shader->set_int("u_TileMap", 1);
	s_data.tilemap_shader->set_int("u_HeightMap", 2);

	s_data.quad_shader = Shader::create("assets/shader/opengl/quad.glsl");
	int sampler_ids[s_data.MAX_TEXTURE_SLOTS];
	for (int i = 0; i < s_data.MAX_TEXTURE_SLOTS; i++)
		sampler_ids[i] = i;
	s_data.quad_shader->set_int_array("u_Textures", sampler_ids, s_data.MAX_TEXTURE_SLOTS);

	s_data.quad_vertex_array = VertexArray::create();
	s_data.quad_vertex_buffer = VertexBuffer::create(s_data.MAX_QUAD_VERTICES * sizeof(QuadVertex));
	s_data.quad_vertex_buffer->set_layout({
		{ ShaderDataType::Vec2, "a_Position" },
		{ ShaderDataType::Vec2, "a_TexCoord" },
		{ ShaderDataType::Int, "a_TexIndex" } });
	s_data.quad_vertex_array->add_vertex_buffer(s_data.quad_vertex_buffer);

	s_data.QUAD_VERTICES = new QuadVertex[s_data.MAX_QUAD_VERTICES];

	uint32_t* const INDICES = new uint32_t[s_data.MAX_QUAD_INDICES];
	uint32_t* indices_ptr = (uint32_t*)INDICES;

	for (uint32_t i = 0, offset = 0; i < s_data.MAX_QUAD_INDICES; i += 6, offset += 4)
	{			
		*indices_ptr++ = offset + 0; // L
		*indices_ptr++ = offset + 3; //    0 --- 1
		*indices_ptr++ = offset + 2; //    | \ R |
		*indices_ptr++ = offset + 0; // R  | L \ |
		*indices_ptr++ = offset + 1; //    2 --- 3
		*indices_ptr++ = offset + 3; //
	}
	
	s_data.quad_index_buffer = IndexBuffer::create(INDICES, s_data.MAX_QUAD_INDICES);
	s_data.quad_vertex_array->set_index_buffer(s_data.quad_index_buffer);

	delete[] INDICES;
}

void Renderer2D::shutdown()
{
	delete[] s_data.QUAD_VERTICES;
}

void Renderer2D::on_window_resize(uint32_t width, uint32_t height)
{
	RenderCommand::set_viewport(0, 0, width, height);
}

void Renderer2D::begin_scene(const OrthographicCamera& camera)
{
	s_data.quad_shader->set_mat4("u_ViewProjection", camera.get_view_projection_matrix());
	s_data.tilemap_shader->set_mat4("u_ViewProjection", camera.get_view_projection_matrix());

	s_data.quad_vertex_buffer_ptr = (QuadVertex*)s_data.QUAD_VERTICES;
	s_data.quad_index_count = 0;

	s_data.texture_slot_index = 0;
}

void Renderer2D::end_scene()
{
	size_t byte_size = (uint8_t*)s_data.quad_vertex_buffer_ptr - (uint8_t*)s_data.QUAD_VERTICES;
	s_data.quad_vertex_buffer->push_data((void*)s_data.QUAD_VERTICES, byte_size);

	flush();
}

void Renderer2D::flush()
{
	for (uint32_t i = 0; i < s_data.texture_slot_index; i++)
		s_data.texture_slots[i]->bind(i);

	if (s_data.quad_index_count > 0)
	{
		s_data.tilemap_shader->bind();
		RenderCommand::draw_indexed(s_data.quad_vertex_array, 6);
		s_data.quad_index_count -= 6;
	}

	/*if (s_data.quad_index_count > 0)
	{
		s_data.quad_shader->bind();
		RenderCommand::draw_indexed(s_data.quad_vertex_array, s_data.quad_index_count);
	}*/
}

void Renderer2D::flush_and_reset()
{
	RENDERER_INFO("Flush and Reset");
	end_scene();

	s_data.quad_vertex_buffer_ptr = (QuadVertex*)s_data.QUAD_VERTICES;
	s_data.quad_index_count  = 0;

	s_data.texture_slot_index = 0;
}

void Renderer2D::assign_texture_slot(const std::shared_ptr<Texture2D>& texture, uint32_t* tex_index)
{
	// check whether new texture matches with previously inserted ones
	for (uint32_t i = 0; i < s_data.texture_slot_index; i++)
	{
		if (s_data.texture_slots[i] == texture)
		{
			*tex_index = i;
			return;
		}
	}

	if (s_data.texture_slot_index >= Renderer2DData::MAX_TEXTURE_SLOTS)
		flush_and_reset();

	// texture_slot_index set to 0 after calling flush_and_reset()
	*tex_index = s_data.texture_slot_index;
	s_data.texture_slots[*tex_index] = texture;
	s_data.texture_slot_index++;
}

void Renderer2D::draw_tilemap(
	glm::vec2 position,
	glm::vec2 size,
	const std::shared_ptr<Texture2D> atlas,
	const std::shared_ptr<Texture2D> tilemap,
	const std::shared_ptr<Texture2D> heightmap)
{
	glm::vec2 QUAD_VERTEX_OFFSETS[4] = {
		{ -(size.x / 2.0f) + (size.x / 3.0f), (size.y / 2.0f) }, { (size.x / 2.0f), (size.y / 2.0f) },
		{ -(size.x / 2.0f), -(size.y / 2.0f) }, { (size.x / 2.0f) - (size.x / 3.0f), -(size.y / 2.0f) } };

	/*glm::vec2 QUAD_VERTEX_OFFSETS[4] = {
		{ -(size.x / 2.0f), (size.y / 2.0f) }, { (size.x / 2.0f), (size.y / 2.0f) },
		{ -(size.x / 2.0f), -(size.y / 2.0f) }, { (size.x / 2.0f), -(size.y / 2.0f) } };*/

	constexpr glm::vec2 QUAD_TEX_COORDS[4] =
	{ { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } };

	if (s_data.quad_index_count + 6 >= Renderer2DData::MAX_QUAD_INDICES)
	{
		RENDERER_INFO("Buffer size exceeded. Flushing before drawing next quad.");
		flush_and_reset();
	}

	uint32_t atlas_tex_index, tilemap_tex_index, heightmap_tex_index;
	assign_texture_slot(atlas, &atlas_tex_index);
	assign_texture_slot(tilemap, &tilemap_tex_index);
	assign_texture_slot(heightmap, &heightmap_tex_index);

	for (int i = 0; i < 4; i++)
	{
		s_data.quad_vertex_buffer_ptr->pos = position + (QUAD_VERTEX_OFFSETS[i]);
		s_data.quad_vertex_buffer_ptr->tex = QUAD_TEX_COORDS[i];
		s_data.quad_vertex_buffer_ptr->tex_index = 0;
		s_data.quad_vertex_buffer_ptr++;
	}

	s_data.quad_index_count += 6;
}

void Renderer2D::draw_quad(
	const glm::vec2 position,
	const std::shared_ptr<Texture2D> texture)
{
	// quad edges offset to quad position
	// the quads origin position lays centered on its bottom line

	// 0 - 1
	// | \ |
	// 2 x 3

	constexpr glm::vec2 VERTEX_OFFSETS[4] = { 
		{ -0.5f, 1.0f }, { 0.5f, 1.0f }, 
		{ -0.5f, 0.0f }, { 0.5f, 0.0f } };

	constexpr glm::vec2 TEX_COORDS[4] = { 
		{ 0.0f, 0.0f }, { 1.0f, 0.0f }, 
		{ 0.0f, 1.0f }, { 1.0f, 1.0f } };

	if (s_data.quad_index_count + 6 >= Renderer2DData::MAX_QUAD_INDICES)
	{
		RENDERER_WARN("Buffer size exceeded. Flushing before drawing next quad.");
		flush_and_reset();
	}

	uint32_t tex_index;
	assign_texture_slot(texture, &tex_index); // calls flush_and_reset() if all slots are occupied

	glm::vec2 texture_size((float)texture->width(), (float)texture->height());
	
	for (int i = 0; i < 4; i++)
	{
		s_data.quad_vertex_buffer_ptr->pos = 
			position + (VERTEX_OFFSETS[i] * texture_size);
		s_data.quad_vertex_buffer_ptr->tex = TEX_COORDS[i];
		s_data.quad_vertex_buffer_ptr->tex_index = tex_index;

		s_data.quad_vertex_buffer_ptr++;
	}

	s_data.quad_index_count += 6;
}
