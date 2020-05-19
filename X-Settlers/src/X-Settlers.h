#pragma once

#include "Core/Window.h"
#include <stdio.h>
#include "Core/Log.h"
#include "Logic/Playfield.h"
#include <memory>

class XSettlers {
public:
	XSettlers();
	void Run();

private:
	bool m_Running;
	std::shared_ptr<Window> m_Window;
	VkBuffer m_Vertices;
	VkBuffer m_Indices;
};