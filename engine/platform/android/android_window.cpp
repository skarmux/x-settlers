#include "android_window.h"

AndroidWindow::AndroidWindow(const WindowProps& props) {
	Initialize(props);
}

AndroidWindow::~AndroidWindow() {
	Shutdown();
}

void AndroidWindow::Initialize(const WindowProps& props) {
	m_Data.Title = "Android Window!"; //props.Title;
	m_Data.Width = props.Width;
	m_Data.Height = props.Height;

    m_Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(m_Display, nullptr, nullptr);
	
    /* Here, the application chooses the configuration it desires.
     * find the best match if possible, otherwise use the very first one
     */
    eglChooseConfig(m_Display, attribs, nullptr,0, &numConfigs);
    std::unique_ptr<EGLConfig[]> supportedConfigs(new EGLConfig[numConfigs]);
    assert(supportedConfigs);
    eglChooseConfig(m_Display, attribs, supportedConfigs.get(), numConfigs, &numConfigs);
    assert(numConfigs);
    auto i = 0;
    for (; i < numConfigs; i++) {
        auto& cfg = supportedConfigs[i];
        EGLint r, g, b, d;
        if (eglGetConfigAttrib(m_Display, cfg, EGL_RED_SIZE, &r)   &&
            eglGetConfigAttrib(m_Display, cfg, EGL_GREEN_SIZE, &g) &&
            eglGetConfigAttrib(m_Display, cfg, EGL_BLUE_SIZE, &b)  &&
            eglGetConfigAttrib(m_Display, cfg, EGL_DEPTH_SIZE, &d) &&
            r == 8 && g == 8 && b == 8 && d == 0 ) {

            config = supportedConfigs[i];
            break;
        }
    }
    if (i == numConfigs) {
        config = supportedConfigs[0];
    }

    if (config == nullptr) {
        //LOGW("Unable to initialize EGLConfig");
        return -1;
    }

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(m_Display, config, EGL_NATIVE_VISUAL_ID, &format);
    m_Window = eglCreateWindowSurface(m_Display, config, engine->app->window, nullptr);
    m_Context = eglCreateContext(m_Display, config, nullptr, nullptr);

    if (eglMakeCurrent(m_Display, m_Surface, m_Surface, m_Context) == EGL_FALSE) {
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(m_Display, m_Surface, EGL_WIDTH, &w);
    eglQuerySurface(m_Display, m_Surface, EGL_HEIGHT, &h);
}

void AndroidWindow::Shutdown() {
	if (m_Display != EGL_NO_DISPLAY) {
        eglMakeCurrent(m_Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (m_Context != EGL_NO_CONTEXT) {
            eglDestroyContext(m_Display, m_Context);
        }
        if (m_Surface != EGL_NO_SURFACE) {
            eglDestroySurface(m_Display, m_Surface);
        }
        eglTerminate(m_Display);
    }
}

void AndroidWindow::OnUpdate() {
	//glfwPollEvents();
	//SDL_PollEvent();
	//glfwSwapBuffers(m_Window);
	//SDL_GL_SwapWindow(m_Window);
    // Event Polling
    // int ident;
    // int events;
    // struct android_poll_source* source;

    // // We loop until all events are read, then continue
    // // to draw the next frame of animation.
    // while ((ident=ALooper_pollAll(0, nullptr, &events, (void**)&source)) >= 0)
    // {
    //     // Process this event.
    //     if (source != nullptr) {
    //         source->process(state, source);
    //     }

    //     // Check if we are exiting.
    //     if (state->destroyRequested != 0) {
    //         Renderer2D::Shutdown();
    //         win->Shutdown();
    //         //engine_term_display(&engine);
    //         return;
    //     }
    // }
    eglSwapBuffers(m_Display, m_Surface);
}

void AndroidWindow::SetVSync(bool enabled) {
	// if (enabled) {
	// 	//glfwSwapInterval(1);
	// 	SDL_GL_SetSwapInterval(1);
	// }	
	// else {
	// 	//glfwSwapInterval(0);
	// 	SDL_GL_SetSwapInterval(0);
	// }

	// m_Data.VSync = enabled;
}

bool AndroidWindow::IsVSync() const {
	return m_Data.VSync;
}
