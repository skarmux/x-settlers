#include "OpenGLRendererAPI.h"

#include "Core/Log.h"
#include "OpenGLShader.h"

#include <SDL.h>
#include <glad/glad.h>

static int shaderProgram;
static unsigned int VBO, VAO, EBO;

void OpenGLRendererAPI::Init()
{
	// initialize glad
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		CORE_ERROR("Failed to initialize glad!");
		exit(2);
	}

    auto shader = OpenGLShader::Create("assets/shaders/OpenGL/Texture.glsl");
    shader->Bind();

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    // ------------------------------------------------------------------



	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glClearColor(0.2f, 0.2f, 0.2f, 1);
}

void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	glViewport(x, y, width, height);
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT); // | GL_DEPTH_BUFFER_BIT
}

void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<Vertex>& vertexArray, uint32_t indexCount)
{
	//uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
	//glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	//glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLRendererAPI::DrawIndexed()
{
    //uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
    //glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    //glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}