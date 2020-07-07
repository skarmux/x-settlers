#pragma once

#include <engine.h>

class Landscape : public Layer
{
public:
	Landscape();
	virtual ~Landscape() = default;

	virtual void on_attach() override;
	virtual void on_detach() override;

	void on_update(TimeDelta ts) override;
	void on_event(Event& e) override;

private:
	OrthographicCameraController m_camera_controller;

	std::shared_ptr<Texture2D> m_grass_texture;
	std::shared_ptr<VertexArray> m_vertex_array;
};