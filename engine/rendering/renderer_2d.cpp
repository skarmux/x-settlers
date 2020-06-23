#include "rendering/renderer_2d.h"
#include "rendering/render_command.h"
#include "rendering/shader.h"
#include "rendering/vertex_array.h"
#include "rendering/buffer.h"

#include "core/log.h"

#include <glm/gtc/matrix_transform.hpp>

struct QuadVertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoord;
};

struct Renderer2DData
{
	static const uint32_t MaxQuads = 20000;
	static const uint32_t MaxVertices = MaxQuads * 4;
	static const uint32_t MaxIndices = MaxQuads * 6;
	static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

	std::shared_ptr<VertexArray> QuadVertexArray;
	std::shared_ptr<VertexBuffer> QuadVertexBuffer;
	std::shared_ptr<Shader> TextureShader;
	std::shared_ptr<Texture2D> WhiteTexture;

	uint32_t QuadIndexCount = 0;
	QuadVertex* QuadVertexBufferBase = nullptr;
	QuadVertex* QuadVertexBufferPtr = nullptr;

	std::array<std::shared_ptr<Texture2D>, MaxTextureSlots> TextureSlots;
	uint32_t TextureSlotIndex = 1; // 0 = white texture

	glm::vec2 QuadVertexPositions[4];

	//Renderer2D::Statistics Stats;
};

static Renderer2DData s_Data;

void Renderer2D::Init()
{
	s_Data.QuadVertexArray = VertexArray::Create();
	
	// TODO: select smallest buffer size
	s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
	s_Data.QuadVertexBuffer->SetLayout({
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" },
		{ ShaderDataType::Float2, "a_TexCoord" } });

	s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

	s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

	//===========================================================================

	uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

	// generate quad indices
	uint32_t offset = 0;
	for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
	{
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;

		quadIndices[i + 3] = offset + 2;
		quadIndices[i + 4] = offset + 3;
		quadIndices[i + 5] = offset + 0;

		offset += 4;
	}

	std::shared_ptr<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
	s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
	delete[] quadIndices;

	//===========================================================================

	s_Data.TextureShader = Shader::Create("assets/shaders/opengl/texture.glsl");
	s_Data.TextureShader->Bind();
	//s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

	// set quad edges offset to quad position
	s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f }; // bottom left
	s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f }; // bottom right
	s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f }; // top right
	s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f }; // top left
}

void Renderer2D::Shutdown()
{
	delete[] s_Data.QuadVertexBufferBase;
}

void Renderer2D::BeginScene(const OrthographicCamera& camera)
{
	s_Data.TextureShader->Bind();
	s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	
	s_Data.QuadIndexCount = 0;
	s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
}

void Renderer2D::EndScene()
{
	// declare size/amount of data within the QuadVertexBuffer
	uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
	s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
	
	Flush();
}

void Renderer2D::Flush()
{
	if (s_Data.QuadIndexCount == 0) {
		RENDERER_WARN("Nothing to draw.");
		return;
	}
	RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
}

void Renderer2D::FlushAndReset()
{
	RENDERER_INFO("Flush and Reset");
	EndScene();

	s_Data.QuadIndexCount = 0;
	s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
}

//void Renderer2D::DrawTriangle(...) { }

void Renderer2D::DrawQuad(const glm::vec2& position, const std::shared_ptr<Texture2D>& texture)
{
	constexpr size_t quadVertexCount = 4;
	constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	
	if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
		RENDERER_INFO("Buffer size exceeded. Flushing before drawing next quad.");
		FlushAndReset();
	}

	// transform default quad positions based on #position and #scale
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));

	for (int i = 0; i < quadVertexCount; i++) {
		s_Data.QuadVertexBufferPtr->Position = transform * glm::vec4(s_Data.QuadVertexPositions[i], 0.0f, 1.0f);
		s_Data.QuadVertexBufferPtr->Color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
		s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
		s_Data.QuadVertexBufferPtr++;
	}

	s_Data.QuadIndexCount += 6;
}

void Renderer2D::OnWindowResize(uint32_t width, uint32_t height)
{
	RenderCommand::SetViewport(0, 0, width, height);
}
