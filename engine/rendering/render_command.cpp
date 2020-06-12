#include "render_command.h"

std::unique_ptr<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
