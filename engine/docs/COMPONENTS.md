## Segments
* core
* events
* platform
* rendering

---

## Core
* application
    > Application superclass that initializes a basic runtime environment with logging, window and renderer. Game applications will extend this class and add game logic.
* entry_point
    > Contains the main class that starts the project by creating an instance of (game-) application class and calling its Run() function.
    ONLY TO BE INCLUDED ONCE BY GAME APPLICATION CLASS !
* input
    > ...
* key_codes
    > Setting up unique key codes for use within the engine. Key codes from external librarys will be mapped to these.
* layer
    > Layer class that implements update and event handling functions for each "layer" of an application. For example: gui and game world are two separate layers, the gui being in front of the game world.
* layer_stack
    > Orders layers in a top to bottom order. Layers can be added or removed to or from the stack. Can be iterated thorugh in order to process input events (ex: mouse click) from top to bottom to be handled by the first layer that handles given event.
* log
    > Implementation of external library spdlog with macros for easier log entry generation. Debugging only.
* timestep
    > An abstracted delta time for frame timings and game logic.

---

## Events
* app_event
    > Window and application runtime events. WindowClosed, , -Resized, AppRender, -Tick, -Update.
* event
    > Base event class.
* key_event
    > All keyboard related events like KeyPressed and KeyReleased.
* mouse_event
    > All mouse related events. Moving, scrolling and the clicking of mouse buttons.

---

## Platform
* window
    > Base window class for platform specific windows or surfaces (on android). Takes resolution and title as metadata. 
* filesystem
    > ...

---

## Rendering
* buffer
    > Stores vertex or index data.
* graphics_context
    > Render context given by window framweork. Framebuffer target.
* render_command
    > Render commands that get passed through to on of the specified rendering_api backend calls for OpenGL, Vulkan or else.
* renderer_2d
    > Handles scene construction, buffers, textures and calls render_commands in order to draw given scene. Has function for more complex draw commands like for quads, sprites and more.
* renderer_api
    > Abstracted interface between render_command and renderer_api's specific interfaces like opengl_renderer_api or vulkan_renderer_api.
* shader
    > Base shader class.
* texture
    > ...
* vertex
    > Vertex structure.