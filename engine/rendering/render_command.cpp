#include "render_command.h"

std::unique_ptr<RendererAPI> RenderCommand::s_renderer_api = RendererAPI::create();
