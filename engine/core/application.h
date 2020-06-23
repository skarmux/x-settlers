
#include "core/layer_stack.h"
#include "events/event.h"
#include "events/app_event.h"
#include "core/window.h"

int main(int argc, char** argv);

class Application
{
public:
    Application();
    virtual ~Application();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    Window& GetWindow() { return *m_Window; }

    static Application& Get() { return *s_Instance; }

private:
    void Run();
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);

private:
    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
    bool m_Minimized = false;
    LayerStack m_LayerStack;
    float m_LastFrameTime = 0.0f;

private:
    static Application* s_Instance;
    friend int ::main(int argc, char** argv);
};

// To be defined in CLIENT
Application* CreateApplication();
