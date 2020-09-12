#pragma once

#include "core/application.h"
#include "core/logger.h"

extern Application* create_application();

int main(int argc, char** argv)
{
	Logger::init();
	Filesystem::init();

	auto application = create_application();

	application->run();

	delete application;

	return 0;
}

//extern AndroidApplication* create_android_application();
//
//void android_main(struct android_app* app)
//{
//	struct engine {
//        struct android_app* app;
//
//        ASensorManager* sensorManager;
//        const ASensor* accelerometerSensor;
//        ASensorEventQueue* sensorEventQueue;
//
//        int animating;
//        EGLDisplay display;
//        EGLSurface surface;
//        EGLContext context;
//        int32_t width;
//        int32_t height;
//        struct saved_state state;
//    };
//    
//    Hazel::Log::Init();
//
//	auto application = Hazel::CreateAndroidApplication(app);
//
//	application->Run();
//
//	delete application;
//}
