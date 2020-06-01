#include "RenderCommand.h"

std::unique_ptr<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
