#include "platform/windows/windows_window.h"

#include "events/event.h"
#include "events/application_event.h"
#include "events/mouse_event.h"

LRESULT CALLBACK WindowsWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //CORE_INFO("received windows event: {0}", uMsg);
    
    WindowsWindow* obj_ptr;

    switch (uMsg)
    {
    //case WM_SIZE:
    //    CORE_INFO("received WM_SIZE event");
    //    switch ((UINT)wParam)
    //    {
    //    case SIZE_MAXIMIZED:
    //        break;
    //    case SIZE_MINIMIZED:
    //        break;
    //    case SIZE_RESTORED: // RESIZED; Neither MAXI- nor MINIMIZED
    //        obj_ptr->m_event_callback(WindowResizeEvent(LOWORD(lParam), HIWORD(lParam)));
    //        break;
    //    }

    case WM_DESTROY:
        CORE_INFO("received WM_DESTROY event");
        obj_ptr = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        obj_ptr->m_event_callback(WindowCloseEvent());
        return 0;

    /*case WM_CLOSE:
        CORE_INFO("received WM_CLOSE event");
        obj_ptr->m_event_callback(WindowCloseEvent());
        DestroyWindow(hwnd);
        return 0;*/

    case WM_PAINT:
        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);

        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam); // default OS behavior
}

WindowsWindow::WindowsWindow(const WindowProps& props)
    : m_title(props.title), m_width(props.width), m_height(props.height)
{
    // Register the window class.
    const LPCSTR CLASS_NAME = "Desktop Window Class";

    WNDCLASS wcex = { };
    wcex.lpfnWndProc = WindowsWindow::WindowProc;
    wcex.hInstance = GetModuleHandle(nullptr);
    wcex.lpszClassName = CLASS_NAME;

    RegisterClass(&wcex);

    m_window = CreateWindowEx(
        0,                         // Optional window styles.
        CLASS_NAME,                // Window class
        (LPCSTR)m_title.c_str(),   // Window text
        WS_OVERLAPPEDWINDOW,       // Window style
        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, m_width, m_height,
        NULL,               // Parent window    
        NULL,               // Menu
        GetModuleHandle(nullptr), // Instance handle
        NULL // Additional application data
    );

    if (m_window == NULL)
    {
        CORE_ERROR("WindowsWindow could not be created!");
        return;
    }

    SetWindowLongPtr(m_window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    set_vsync(true);

    ShowWindow(m_window, 1);
}

WindowsWindow::~WindowsWindow()
{
    PostQuitMessage(0);
}

void WindowsWindow::on_update()
{
    UpdateWindow(m_window);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg); // translates key strokes to chars
        DispatchMessage(&msg); // invokes WindProc CALLBACK func
    }
}

float WindowsWindow::get_time() const
{
	return 0.0f; // TODO: WindowsWindow::get_time()
}

void WindowsWindow::set_vsync(bool enabled)
{
    // TODO: WindowsWindow::set_vsync(bool enabled)
}