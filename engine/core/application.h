#pragma once

#include "core/layer_stack.h"
#include "core/window.h"

#include "events/event.h"
#include "events/application_event.h"

int main(int argc, char** argv);

class Application
{
public:
    Application(const std::string& name, uint32_t width, uint32_t height);
    virtual ~Application();

    void push_layer(Layer* layer);

    Window& get_window() { return *m_window; }

    static Application& get_instance() { return *s_instance; }
    
    void run();
private:
    void on_event(Event& e);
    bool on_window_close(WindowCloseEvent& e);
    bool on_window_resize(WindowResizeEvent& e);
private:
    std::unique_ptr<Window> m_window;
    bool m_running = true;
    bool m_minimized = false;
    LayerStack m_layer_stack;
    float m_last_frame_time = 0.0f;
private:
    static Application* s_instance;
    friend int ::main(int argc, char** argv);
};

// To be defined in CLIENT
Application* create_application();