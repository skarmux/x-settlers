#include "rendering/renderer_2d.h"
#include "rendering/render_command.h"
#include "rendering/shader.h"
#include "rendering/vertex_array.h"
#include "rendering/buffer.h"

#define max(a,b) a > b ? a : b
#define min(a,b) a < b ? a : b

struct Renderer2DData
{
	static const uint32_t MAX_TRIS = 500000;
	static const uint32_t MAX_TRI_VERTICES = MAX_TRIS * 3;
	
	static const uint32_t MAX_QUADS = 20000;
	static const uint32_t MAX_QUAD_VERTICES = MAX_QUADS * 4;
	static const uint32_t MAX_QUAD_INDICES = MAX_QUADS * 6;

	static const uint32_t MAX_RHOMB = 20000;
	static const uint32_t MAX_RHOMB_VERTICES = MAX_RHOMB * 4;
	static const uint32_t MAX_RHOMB_INDICES = MAX_RHOMB * 6;

	static const uint32_t MAX_TEXTURE_SLOTS = 32;
	std::array<std::shared_ptr<Texture2D>, MAX_TEXTURE_SLOTS> texture_slots;
	uint32_t texture_slot_index = 1;

	std::shared_ptr<Shader> tri_shader;
	std::shared_ptr<Shader> quad_shader;
	std::shared_ptr<Shader> rhomb_shader;

	std::shared_ptr<VertexArray> tri_vertex_array;
	std::shared_ptr<VertexBuffer> tri_vertex_buffer;

	std::shared_ptr<VertexArray> quad_vertex_array;
	std::shared_ptr<IndexBuffer> quad_index_buffer;
	std::shared_ptr<VertexBuffer> quad_vertex_buffer;

	std::shared_ptr<VertexArray> rhomb_vertex_array;
	std::shared_ptr<IndexBuffer> rhomb_index_buffer;
	std::shared_ptr<VertexBuffer> rhomb_vertex_buffer;

	Renderer2D::TriVertex* TRI_VERTICES;
	Renderer2D::QuadVertex* QUAD_VERTICES;
	Renderer2D::RhombVertex* RHOMB_VERTICES;

	Renderer2D::TriVertex* tri_vertex_buffer_ptr     = nullptr;
	Renderer2D::QuadVertex* quad_vertex_buffer_ptr   = nullptr;
	Renderer2D::RhombVertex* rhomb_vertex_buffer_ptr = nullptr;

	uint32_t tri_vertex_count  = 0;
	uint32_t quad_index_count  = 0;
	uint32_t rhomb_index_count = 0;
};

static Renderer2DData s_data;

void Renderer2D::init()
{
	auto fallback_texture = Texture2D::create(1, 1);
	uint32_t magenta_rgba = 0xffff00ff;
	fallback_texture->push_data(&magenta_rgba, sizeof(magenta_rgba));
	s_data.texture_slots[0] = fallback_texture;

	int32_t sampler_ids[s_data.MAX_TEXTURE_SLOTS];
	for (uint32_t i = 0; i < s_data.MAX_TEXTURE_SLOTS; i++)
		sampler_ids[i] = i;

	s_data.tri_shader = Shader::create("assets/shader/opengl/triangle.glsl");
	s_data.quad_shader = Shader::create("assets/shader/opengl/quad.glsl");
	s_data.rhomb_shader = Shader::create("assets/shader/opengl/mesh_experimental.glsl");

	s_data.tri_shader->set_int_array(
		"u_Textures", sampler_ids, s_data.MAX_TEXTURE_SLOTS);
	s_data.quad_shader->set_int_array(
		"u_Textures", sampler_ids, s_data.MAX_TEXTURE_SLOTS);
	s_data.rhomb_shader->set_int_array(
		"u_Textures", sampler_ids, s_data.MAX_TEXTURE_SLOTS);
	
	s_data.tri_vertex_array = VertexArray::create();
	s_data.tri_vertex_buffer = VertexBuffer::create(
		s_data.MAX_TRI_VERTICES * sizeof(TriVertex));
	s_data.tri_vertex_buffer->set_layout({
		{ ShaderDataType::Float2, "a_Position" },
		{ ShaderDataType::Float2, "a_TexCoord" },
		{ ShaderDataType::Float1, "a_TexIndex" },
		{ ShaderDataType::Float1, "a_Shade"    } });
	s_data.tri_vertex_array->add_vertex_buffer(s_data.tri_vertex_buffer);

	s_data.quad_vertex_array = VertexArray::create();
	s_data.quad_vertex_buffer = VertexBuffer::create(
		s_data.MAX_QUAD_VERTICES * sizeof(QuadVertex));
	s_data.quad_vertex_buffer->set_layout({
		{ ShaderDataType::Float2, "a_Position" },
		{ ShaderDataType::Float2, "a_TexCoord" },
		{ ShaderDataType::Float1, "a_TexIndex" } });
	s_data.quad_vertex_array->add_vertex_buffer(s_data.quad_vertex_buffer);

	s_data.rhomb_vertex_array = VertexArray::create();
	s_data.rhomb_vertex_buffer = VertexBuffer::create(
		s_data.MAX_RHOMB_VERTICES * sizeof(RhombVertex));
	s_data.rhomb_vertex_buffer->set_layout({
		{ ShaderDataType::Float2, "a_Position" },
		{ ShaderDataType::Float2, "a_TexCoord" },
		{ ShaderDataType::Float1, "a_Shade"    },
		{ ShaderDataType::Float1, "a_TexIndex" } });
	s_data.rhomb_vertex_array->add_vertex_buffer(s_data.rhomb_vertex_buffer);

	s_data.TRI_VERTICES = new TriVertex[s_data.MAX_TRI_VERTICES];
	s_data.QUAD_VERTICES = new QuadVertex[s_data.MAX_QUAD_VERTICES];
	s_data.RHOMB_VERTICES = new RhombVertex[s_data.MAX_RHOMB_VERTICES];

	{
		uint32_t* const INDICES = new uint32_t[max(s_data.MAX_QUAD_INDICES, s_data.MAX_RHOMB_INDICES)];
		uint32_t* indices_ptr = (uint32_t*)INDICES;

		for (uint32_t i = 0, offset = 0; i < s_data.MAX_QUAD_INDICES; i += 6, offset += 4)
		{			
			*indices_ptr++ = offset + 0; 
			*indices_ptr++ = offset + 1; // 0 --- 3
			*indices_ptr++ = offset + 2; // | \ B |
			*indices_ptr++ = offset + 2; // | A \ |
			*indices_ptr++ = offset + 3; // 1 --- 2
			*indices_ptr++ = offset + 0;
		}
		
		s_data.quad_index_buffer = IndexBuffer::create(INDICES, s_data.MAX_QUAD_INDICES);
		s_data.quad_vertex_array->set_index_buffer(s_data.quad_index_buffer);

		s_data.rhomb_index_buffer = IndexBuffer::create(INDICES, s_data.MAX_RHOMB_INDICES);
		s_data.rhomb_vertex_array->set_index_buffer(s_data.rhomb_index_buffer);

		delete[] INDICES;
	}
}

void Renderer2D::shutdown()
{
	delete[] s_data.TRI_VERTICES;
	delete[] s_data.QUAD_VERTICES;
	delete[] s_data.RHOMB_VERTICES;
}

void Renderer2D::on_window_resize(uint32_t width, uint32_t height)
{
	RenderCommand::set_viewport(0, 0, width, height);
}

void Renderer2D::begin_scene(const OrthographicCamera& camera)
{
	s_data.tri_shader->set_mat4("u_ViewProjection", camera.get_view_projection_matrix());
	s_data.quad_shader->set_mat4("u_ViewProjection", camera.get_view_projection_matrix());
	s_data.rhomb_shader->set_mat4("u_ViewProjection", camera.get_view_projection_matrix());
	
	s_data.tri_vertex_buffer_ptr   = s_data.TRI_VERTICES;
	s_data.quad_vertex_buffer_ptr  = s_data.QUAD_VERTICES;
	s_data.rhomb_vertex_buffer_ptr = s_data.RHOMB_VERTICES;

	s_data.tri_vertex_count   = 0;
	s_data.quad_index_count   = 0;
	s_data.rhomb_index_count  = 0;

	s_data.texture_slot_index = 1;
}

void Renderer2D::end_scene()
{
	// push vertex data to GPU memory

	uint32_t byte_size =
		(uint8_t*)s_data.tri_vertex_buffer_ptr - (uint8_t*)s_data.TRI_VERTICES;
	s_data.tri_vertex_buffer->push_data(s_data.TRI_VERTICES, byte_size);

	byte_size = 
		(uint8_t*)s_data.quad_vertex_buffer_ptr - (uint8_t*)s_data.QUAD_VERTICES;
	s_data.quad_vertex_buffer->push_data(s_data.QUAD_VERTICES, byte_size);

	byte_size =
		(uint8_t*)s_data.rhomb_vertex_buffer_ptr - (uint8_t*)s_data.RHOMB_VERTICES;
	s_data.rhomb_vertex_buffer->push_data(s_data.RHOMB_VERTICES, byte_size);

	flush();
}

void Renderer2D::flush()
{
	if (s_data.quad_index_count == 0 
		&& s_data.rhomb_index_count == 0 
		&& s_data.tri_vertex_count == 0)
	{
		RENDERER_WARN("Nothing to draw.");
		return;
	}

	for (uint32_t i = 0; i < s_data.texture_slot_index; i++)
		s_data.texture_slots[i]->bind(i);

	s_data.tri_shader->bind();
	RenderCommand::draw(s_data.tri_vertex_array, s_data.tri_vertex_count);

	s_data.quad_shader->bind();
	RenderCommand::draw_indexed(s_data.quad_vertex_array, s_data.quad_index_count);

	s_data.rhomb_shader->bind();
	RenderCommand::draw_indexed(s_data.rhomb_vertex_array, s_data.rhomb_index_count);
}

void Renderer2D::flush_and_reset()
{
	RENDERER_INFO("Flush and Reset");
	end_scene();
	
	s_data.tri_vertex_buffer_ptr   = s_data.TRI_VERTICES;
	s_data.quad_vertex_buffer_ptr  = s_data.QUAD_VERTICES;
	s_data.rhomb_vertex_buffer_ptr = s_data.RHOMB_VERTICES;

	s_data.tri_vertex_count  = 0;
	s_data.quad_index_count  = 0;
	s_data.rhomb_index_count = 0;

	s_data.texture_slot_index = 1;
}

//void Renderer2D::draw_rhomboid_mesh(
//	uint32_t width, float x_step, float y_step, float per_row_offset,
//	const std::vector<std::shared_ptr<Texture2D>>& textures)
//{
//	if (width > s_data.MAX_MESH_WIDTH)
//		RENDERER_ERROR(
//			"Cannot draw mesh. Width exceeds maximum allowed. {0} > {1}",
//			width, s_data.MAX_MESH_WIDTH);
//
//	// push new textures
//	for (auto texture : textures)
//	{
//		float tex_index = 0.0f;
//
//		for (uint32_t i = 1; i < s_data.texture_slot_index; i++)
//		{
//			if (s_data.texture_slots[i] == texture)
//			{
//				// texture already slotted
//				tex_index = (float)i;
//				break;
//			}
//		}
//
//		if (tex_index == 0.0f)
//		{
//			// add texture to slots
//			if (s_data.texture_slot_index >= Renderer2DData::MAX_TEXTURE_SLOTS)
//				flush_and_reset();
//
//			tex_index = (float)s_data.texture_slot_index;
//			s_data.texture_slots[s_data.texture_slot_index] = texture;
//			s_data.texture_slot_index++;
//		}
//	}
//
//	uint32_t padding = (s_data.MAX_MESH_WIDTH - width) / 2;
//	s_data.rhomb_vertex_buffer_ptr = s_data.RHOMB_VERTICES + padding + (s_data.MAX_MESH_WIDTH * padding);
//
//	for (uint32_t row = padding; row < s_data.MAX_MESH_WIDTH - padding; row++)
//	{
//		for (uint32_t col = padding; col < s_data.MAX_MESH_WIDTH - padding; col++)
//		{
//			s_data.rhomb_vertex_buffer_ptr->pos.x = 
//				(float)(col * x_step) + ((s_data.MAX_MESH_WIDTH - row)*(per_row_offset));
//			s_data.rhomb_vertex_buffer_ptr->pos.y = 
//				(float)(s_data.MAX_MESH_WIDTH - row) * y_step;
//			s_data.rhomb_vertex_buffer_ptr->tex = { 
//				s_data.rhomb_vertex_buffer_ptr->pos.x / 128.0f,
//				s_data.rhomb_vertex_buffer_ptr->pos.y / 128.0f };
//			s_data.rhomb_vertex_buffer_ptr->pos.y += 
//				(float)area[(width * (row - padding)) + col - padding].height;
//
//			/* Soil = 1, Bricks = 2, Sand = 3, Sea = 7, Grass = 16,
//			Rock = 32, SandTrace = 35, DrySandTrace = 36, Beach = 48,
//			Desert = 64, Swamp = 80, Ice = 128, Mud = 144, DarkSoil = 217,
//			Cement = 230 */
//
//			switch (area[(width * (row - padding)) + col - padding].type)
//			{
//			case 0:
//			case 1: s_data.rhomb_vertex_buffer_ptr->tex_index = 1; break; // grass
//			case 2: 
//			case 3: s_data.rhomb_vertex_buffer_ptr->tex_index = 3; break; // sand
//			case 7: s_data.rhomb_vertex_buffer_ptr->tex_index = 4; break; // water
//			case 10:
//			case 16: s_data.rhomb_vertex_buffer_ptr->tex_index = 1; break; // grass
//			case 17: s_data.rhomb_vertex_buffer_ptr->tex_index = 4; break; // water
//			case 21:
//			case 32: s_data.rhomb_vertex_buffer_ptr->tex_index = 2; break; // rock
//			case 35:
//			case 36:
//			case 48:
//			case 64: s_data.rhomb_vertex_buffer_ptr->tex_index = 3; break; // sand
//			default: s_data.rhomb_vertex_buffer_ptr->tex_index = 1;
//			}
//			
//			
//			if (row < s_data.MAX_MESH_WIDTH - padding - 1)
//			{
//				uint8_t top = area[(width * (row - padding)) + col - padding].height;
//				uint8_t left = area[(width * (row - padding + 1)) + col - padding].height;
//				s_data.rhomb_vertex_buffer_ptr->col =
//					0.875f + (float)(top - left) * 0.1f;
//			}
//			else
//				s_data.rhomb_vertex_buffer_ptr->col = 1.0f;
//
//			s_data.rhomb_vertex_buffer_ptr->pos.x -= 
//				(float)s_data.MAX_MESH_WIDTH * x_step * 1.5f / 2.0;
//			s_data.rhomb_vertex_buffer_ptr->pos.y -= 
//				(float)s_data.MAX_MESH_WIDTH * y_step * 0.5f;
//
//			s_data.rhomb_vertex_buffer_ptr++;
//		}
//		s_data.rhomb_vertex_buffer_ptr += (2*padding);
//	}
//}

void Renderer2D::assign_texture_slot(const std::shared_ptr<Texture2D>& texture, uint32_t& tex_index)
{
	tex_index = 0;
	
	// check whether new texture matches with previously inserted ones
	for (uint32_t i = 1; i < s_data.texture_slot_index; i++)
	{
		if (s_data.texture_slots[i] == texture)
		{
			tex_index = i;
			return;
		}
	}

	if (s_data.texture_slot_index >= Renderer2DData::MAX_TEXTURE_SLOTS)
		flush_and_reset();

	// (texture_slot_index = 1) after calling flush_and_reset()
	tex_index = s_data.texture_slot_index;
	s_data.texture_slots[tex_index] = texture;
	s_data.texture_slot_index++;
}

void Renderer2D::draw_triangle(
	const glm::vec2 positions[3],
	const glm::vec2 tex_coords[3],
	const float shades[3],
	const std::shared_ptr<Texture2D>& texture)
{
	if (s_data.tri_vertex_count >= Renderer2DData::MAX_TRI_VERTICES)
	{
		RENDERER_INFO("Buffer size exceeded. Flushing before drawing next quad.");
		flush_and_reset();
	}

	uint32_t tex_index = 0;
	assign_texture_slot(texture, tex_index); // will flush_and_reset() when all slots are occupied

	for (uint32_t i = 0; i < 3; i++)
	{
		s_data.tri_vertex_buffer_ptr->pos       = (glm::vec2)positions[i];
		s_data.tri_vertex_buffer_ptr->tex       = tex_coords[i];
		s_data.tri_vertex_buffer_ptr->tex_index = tex_index;
		s_data.tri_vertex_buffer_ptr->shade     = shades[i];
		s_data.tri_vertex_buffer_ptr++;
	}

	s_data.tri_vertex_count += 3;
}

void Renderer2D::draw_quad(
	const glm::vec2 position,
	const std::shared_ptr<Texture2D>& texture,
	const glm::vec2 tex_coords[3])
{
	// quad edges offset to quad position
	// the quads origin position lays centered on its bottom line
	// bl, br, tr, tl
	constexpr glm::vec2 QUAD_VERTEX_OFFSETS[] = 
		{ { -0.5f, 0.0f }, { 0.5f, 0.0f }, { 0.5f, 1.0f }, { -0.5f, 1.0f } };

	if (s_data.quad_index_count >= Renderer2DData::MAX_QUAD_INDICES)
	{
		RENDERER_INFO("Buffer size exceeded. Flushing before drawing next quad.");
		flush_and_reset();
	}

	uint32_t tex_index = 0;
	assign_texture_slot(texture, tex_index); // flush_and_reset() when all slots are occupied

	glm::vec2 texture_size((float)texture->get_width(), (float)texture->get_height());
	
	for (int i = 0; i < 4; i++)
	{
		s_data.quad_vertex_buffer_ptr->pos = 
			(glm::vec2)position + (QUAD_VERTEX_OFFSETS[i] * texture_size);
		s_data.quad_vertex_buffer_ptr->tex = tex_coords[i];
		s_data.quad_vertex_buffer_ptr->tex_index = tex_index;
		s_data.quad_vertex_buffer_ptr++;
	}

	s_data.quad_index_count += 6;
}

void Renderer2D::draw_rhomboid(
	const glm::vec2 position,
	const float top_shift,
	const std::shared_ptr<Texture2D>& texture,
	const glm::vec2* tex_coords)
{
	// top_shift --> 0 --- 3
	//              / \ B /
	//             / A \ /
	//            1 --- 2
	
	const glm::vec2 RHOMB_VERTEX_OFFSETS[] = {
		{0.0f, 0.0f},
		{-top_shift, texture->get_height()},
		{-top_shift + texture->get_width(), texture->get_height()},
		{texture->get_width(), 0.0f} };
	
	if (s_data.rhomb_index_count >= Renderer2DData::MAX_RHOMB_INDICES)
	{
		RENDERER_INFO("Buffer size exceeded. Flushing before drawing next quad.");
		flush_and_reset();
	}

	uint32_t tex_index = 0;
	assign_texture_slot(texture, tex_index); // flush_and_reset() when all slots are occupied

	for (int i = 0; i < 4; i++)
	{
		s_data.rhomb_vertex_buffer_ptr->pos = 
			(glm::vec2)(position + RHOMB_VERTEX_OFFSETS[i]);
		s_data.rhomb_vertex_buffer_ptr->tex = tex_coords[i];
		s_data.rhomb_vertex_buffer_ptr->tex_index = tex_index;
		s_data.rhomb_vertex_buffer_ptr++;
	}

	s_data.rhomb_index_count += 6;
}

void Renderer2D::submit(TriVertex* buffer, uint32_t count, const std::shared_ptr<Texture2D>& texture)
{
	if (s_data.tri_vertex_count >= Renderer2DData::MAX_TRI_VERTICES)
	{
		RENDERER_INFO("Buffer size exceeded. Flushing before drawing next quad.");
		flush_and_reset();
	}

	uint32_t tex_index = 0;
	assign_texture_slot(texture, tex_index); // will flush_and_reset() when all slots are occupied

	/*if (s_data.tri_vertex_count + count > Renderer2DData::MAX_TRI_VERTICES)
	{
		memcpy(s_data.tri_vertex_buffer_ptr, buffer, (Renderer2DData::MAX_TRI_VERTICES - s_data.tri_vertex_count) * sizeof(TriVertex));
		buffer += (Renderer2DData::MAX_TRI_VERTICES - s_data.tri_vertex_count);
		count  -= (Renderer2DData::MAX_TRI_VERTICES - s_data.tri_vertex_count);
		flush_and_reset();
		memcpy(s_data.tri_vertex_buffer_ptr, buffer, count * sizeof(TriVertex));
	}
	else
	{*/
	memcpy(s_data.tri_vertex_buffer_ptr, buffer, count * sizeof(TriVertex));
	//}

	s_data.tri_vertex_buffer_ptr += count;
	s_data.tri_vertex_count += count;
}