#pragma once

#ifdef ANDROID

extern Hazel::AndroidApplication* Hazel::CreateAndroidApplication();

void android_main(struct android_app* app)
{
	struct engine {
        struct android_app* app;

        ASensorManager* sensorManager;
        const ASensor* accelerometerSensor;
        ASensorEventQueue* sensorEventQueue;

        int animating;
        EGLDisplay display;
        EGLSurface surface;
        EGLContext context;
        int32_t width;
        int32_t height;
        struct saved_state state;
    };
    
    Hazel::Log::Init();

	auto application = Hazel::CreateAndroidApplication(app);

	application->Run();

	delete application;
}

#else

extern Application* CreateApplication();

int main(int argc, char** argv)
{
	Log::Init();

	auto application = CreateApplication();
    
	application->Run();

	delete application;

    return 0;
}

#endif