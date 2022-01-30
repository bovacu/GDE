#include <glad/glad.h>
#include "Window.h"
#include "core/render/window/event/WindowEvent.h"
#include "stb_image.h"
#include "core/render/window/input/Input.h"

namespace engine {

    static uint8_t GLFWWindowCount = 0;

    static void GLFWErrorCallback(int _error, const char* _description) {
        LOG_E("GLFW Error (", _error,"): ", _description);
    }

    Window::Window(const WindowProperties& _props) : window(nullptr) {
        init(_props);
    }

    Window::~Window() {
        shutdown();
    }

    void Window::init(const WindowProperties& _props) {
        data.title = _props.title;
        data.width = (int)_props.width;
        data.height = (int)_props.height;

#ifdef ENGINE_DEBUG
        LOG_W("Running in debug mode");
        LOG_I("Creating window ", _props.title, " (", _props.width, _props.height, ")");
#endif

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            return;
        }

        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        /// Initialize GLFW, just for the first window.
        window = SDL_CreateWindow(_props.title.c_str(), 0, 0, (int)_props.width, (int)_props.height, SDL_WINDOW_OPENGL| SDL_WINDOW_ALLOW_HIGHDPI);
        context = SDL_GL_CreateContext(window);

        SDL_GL_MakeCurrent(window, context);
        SDL_GL_SetSwapInterval(1);
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

        gladLoadGLLoader(SDL_GL_GetProcAddress);

        LOG_S("GLEW and SDL2 initiated correctly");
    }

    void Window::setWindowSize(int _width, int _height) {
        data.width = _width;
        data.height = _height;
    }

    void Window::setWindowSizeAndUpdateNativeWindow(int _width, int _height) {
        data.width = _width;
        data.height = _height;
        SDL_SetWindowSize(window, _width, _height);
    }

    void Window::shutdown() {
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
        SDL_Quit();
    }

    void Window::update() {
        SDL_GL_SwapWindow(window);
    }

    void Window::setVSync(bool _enabled) {
        data.vSync = _enabled;
    }

    bool Window::isVSyncActive() const {
        return data.vSync;
    }

    void Window::setFullscreen(bool _fullscreen) {

    }

    bool Window::isFullscreen() const {
        return data.fullscreen;
    }

    void Window::setIcon(const char* _path) {

    }

    void Window::setWindowOptions(WindowOptions _op, bool _allow) {
//        auto _windowHandle = GetForegroundWindow();
//
//        /// GetWindowLongPtr is used instead of GetWindowLong for x64 system compatibilities
//        auto _style = GetWindowLongPtr(_windowHandle, GWL_STYLE);
//
//        /// The style in GLFW works as an int, so to enable or disable a function we add or subtract the desired amount.
//        _style += windowOptionsToGLFW(_op, _allow);
//
//        SetWindowLongPtr(_windowHandle, GWL_STYLE, _style);
    }

    long Window::windowOptionsToGLFW(WindowOptions _options, bool _allow) {
        long _newStyle = 0L;
        int _toAdd = _allow ? 1 : -1;

        /// Checking each of the mask in case the need to be enabled/disabled and adding it to the new style value.
//        if(((unsigned)WindowOptions_::WindowOptions_Minimize & (unsigned)_options) == WindowOptions_::WindowOptions_Minimize)
//            _newStyle += WS_MINIMIZEBOX * _toAdd;
//
//        if(((unsigned)WindowOptions_::WindowOptions_Maximize & (unsigned)_options) == WindowOptions_::WindowOptions_Maximize)
//            _newStyle += WS_MAXIMIZEBOX * _toAdd;
//
//        if(((unsigned)WindowOptions_::WindowOptions_Resize & (unsigned)_options) == WindowOptions_::WindowOptions_Resize)
//            _newStyle += WS_THICKFRAME * _toAdd;
//
//        if(((unsigned)WindowOptions_::WindowOptions_Visible & (unsigned)_options) == WindowOptions_::WindowOptions_Visible)
//            _newStyle += WS_VISIBLE * _toAdd;
//
//        if(((unsigned)WindowOptions_::WindowOptions_Disabled & (unsigned)_options) == WindowOptions_::WindowOptions_Disabled)
//            _newStyle += WS_DISABLED * _toAdd;
//
//        if(((unsigned)WindowOptions_::WindowOptions_H_Scroll & (unsigned)_options) == WindowOptions_::WindowOptions_H_Scroll)
//            _newStyle += WS_HSCROLL * _toAdd;
//
//        if(((unsigned)WindowOptions_::WindowOptions_V_Scroll & (unsigned)_options) == WindowOptions_::WindowOptions_V_Scroll)
//            _newStyle += WS_VSCROLL * _toAdd;

        return _newStyle;
    }

    std::unique_ptr<Window> Window::createWindow(const WindowProperties& _props) {
        return std::make_unique<Window>(_props);
    }

    void Window::setPosition(const Vec2I& _position) {
        data.position = Vec2I(_position.x, _position.y);
    }

    Vec2I Window::getPosition() const {
        return data.position;
    }

    SDL_Window* Window::getNativeWindow() const {
        return window;
    }

    void Window::setEventCallback(const EventCallbackFn& _callback) {
        data.eventCallback = _callback;
    }

    Vec2I Window::getWindowSize() const {
        return  {getWidth(), getHeight()};
    }

    void Window::setTitle(const std::string& _title) {
        data.title = _title;
        SDL_SetWindowTitle(window, _title.c_str());
    }

    std::string& Window::getTitle() {
        return data.title;
    }

    int Window::getHeight() const {
        return data.height;
    }

    int Window::getWidth() const {
        return data.width;
    }

    void Window::swapBuffers() const {
        SDL_GL_SwapWindow(window);
    }

    SDL_GLContext& Window::getContext() {
        return context;
    }

}
