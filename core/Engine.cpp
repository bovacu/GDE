// Created by borja on 24/12/21.

#include "Engine.h"
#include "core/systems/soundSystem/SoundSystem.h"
#include "core/systems/soundSystem/SoundBuffer.h"
#include "core/systems/uiSystem/FontManager.h"
#include "core/render/elements/ShaderManager.h"
#include "sandbox/Sandbox.h"

namespace engine {
    Engine *Engine::gameInstance = nullptr;

    Engine::Engine() {
        ENGINE_ASSERT(!Engine::gameInstance, "Application already exists!");
        Engine::gameInstance = this;
        window = Window::createWindow();
        window->setEventCallback(ENGINE_BIND_EVENT_FN(Engine::onEvent));
        lastFrame = 0;

        InputSystem::get()->init(window.get());
        Console::get().init();
        ShaderManager::get().init();
        FontManager::get().init();
        Renderer::init(window.get());

        imGuiLayer = new ImGuiLayer();
        pushOverlay(imGuiLayer);

        sandbox = new Sandbox;
        pushLayer(sandbox);

        if(timePerFrame < 0)
            timePerFrame = 1.0f / 60.f;

        window->setVSync(false);

        camera.onResize(window->getWindowSize().x, window->getWindowSize().y);

        FrameBufferSpecification _specs = {
                (uint32_t)window->getWindowSize().x,
                (uint32_t)window->getWindowSize().y
        };
        frameBuffer = new FrameBuffer(_specs);

        Console::get().addCommand("background_color", BIND_FUNC_1(Engine::changeColorConsoleCommand));
    }

    Engine::~Engine() {
        SoundSystem::get().clean();
        delete frameBuffer;
    }

    void Engine::onRun() {
        float _accumulator = 0;

        Delta _dt = 0;
        while (this->running) {
            Uint64 _start = SDL_GetPerformanceCounter();
            _accumulator += _dt;

            engine::Profiler::beginFrame(_dt);

            InputSystem::get()->pollEvents();

            if (!this->minimized) {

                Profiler::begin(ProfilerState::FIXED_UPDATE);
                while (_accumulator >= this->timePerFrame) {
                    _accumulator -= this->timePerFrame;
                    this->onFixedUpdate(this->timePerFrame);
                }
                Profiler::end(ProfilerState::FIXED_UPDATE);

                Profiler::begin(ProfilerState::UPDATE);
                this->onUpdate(_dt);
                Profiler::end(ProfilerState::UPDATE);

                Profiler::begin(ProfilerState::RENDERING);
                this->onRender(_dt);
                Profiler::end(ProfilerState::RENDERING);

                #ifdef ENGINE_DEBUG
                this->updateFps();
                #endif
            }

            engine::Profiler::begin(ProfilerState::INPUT);
            this->window->update();
            engine::Profiler::end(ProfilerState::INPUT);

            engine::Profiler::endFrame();
            Renderer::resetDebugInfo();

            Uint64 _end = SDL_GetPerformanceCounter();
            float _elapsedMS = (float)(_end - _start) / (float)SDL_GetPerformanceFrequency();
            _dt = _elapsedMS;
            timer += _dt;

//            // Cap to 60 FPS
//            SDL_Delay(floor(16.666f - elapsedMS));
        }
    }

    void Engine::onEvent(Event &_e) {
        EventDispatcher dispatcher(_e);
        dispatcher.dispatchEvent<WindowClosedEvent>(ENGINE_BIND_EVENT_FN(Engine::onWindowClosed));
        dispatcher.dispatchEvent<WindowResizedEvent>(ENGINE_BIND_EVENT_FN(Engine::onWindowResized));

        // TODO this must be in another layer, this is why the scroll is called event when on ImGui windows
        dispatcher.dispatchEvent<MouseScrolledEvent>(ENGINE_BIND_EVENT_FN(Engine::onMouseScrolled));

        for (auto _it = this->layerStack.rbegin(); _it != this->layerStack.rend(); ++_it) {
            (*_it)->onEvent(_e);
            if (_e.handled)
                break;
        }
    }

    void Engine::onFixedUpdate(Delta _fixedDt) {
        for (Layer* _layer : this->layerStack)
            _layer->onFixedUpdate(_fixedDt);
    }

    void Engine::onUpdate(Delta _dt) {
        auto _fbSpec = frameBuffer->getSpecification();
        if(window->getWindowSize().x > 0 && window->getWindowSize().y > 0 && (_fbSpec.width != window->getWindowSize().x || _fbSpec.height != window->getWindowSize().y)) {
            frameBuffer->resize(window->getWindowSize().x, window->getWindowSize().y);
            this->camera.onResize(window->getWindowSize().x, window->getWindowSize().y);
        }

        for (Layer* _layer : this->layerStack)
            _layer->onUpdate(_dt);

        if(InputManager::isKeyJustPressed(KeyCode::F9))
            showImGuiDebugWindow = !showImGuiDebugWindow;

        camera.setRotation(camera.getRotation() + 5 * _dt);

    }

    void Engine::onRender(Delta _dt) {
        Renderer::clear(backgroundColor);

        // Normal rendering
        Renderer::beginDraw(camera);
        for (Layer* _layer : this->layerStack)
            _layer->onRender(_dt);
        Renderer::endDraw();

        // Debug rendering
        Renderer::beginDebugDraw(camera);
        Renderer::drawSquare({0, 0}, {2, 2}, Color::Blue);
        Renderer::endDebugDraw();

        // Imgui rendering
        Profiler::begin(ProfilerState::IMGUI);
        this->imGuiLayer->begin();
        for (Layer* _layer : this->layerStack)
            _layer->onImGuiRender(_dt);
            if(showImGuiDebugWindow)
                imGuiLayer->drawDebugInfo();
        this->imGuiLayer->end();
        Profiler::end(ProfilerState::IMGUI);
    }

    bool Engine::onMouseScrolled(MouseScrolledEvent& _e) {
        float _zoom = camera.getCurrentZoomLevel();
        _zoom -= _e.getScrollY() * camera.getZoomSpeed();
        _zoom = std::max(_zoom, 0.1f);
        camera.setCurrentZoomLevel(_zoom);

        return false;
    }

    bool Engine::onWindowClosed(WindowClosedEvent &_e) {
        this->running = false;
        return true;
    }

    bool Engine::onWindowResized(WindowResizedEvent &_e) {
        if (_e.getWidth() == 0 || _e.getHeight() == 0) {
            this->minimized = true;
            return false;
        }

        this->minimized = false;

        return false;
    }

    int Engine::getFps() const { return (int)this->fpsCounter; }

    void Engine::updateFps() {
        if (timer >= 1.f) {
            fpsCounter = frameCounter;
            setTitle("Engine: " + std::to_string(fpsCounter));
            frameCounter = 0;
            timer = 0;
        }
        ++frameCounter;
    }

    void Engine::setTitle(const std::string& _title) {
        this->window->setTitle(_title);
    }

    void Engine::setFullscreen(bool _fullscreen) {
        this->window->setFullscreen(_fullscreen);
    }

    void Engine::setVSync(bool _vsync) {
        this->window->setVSync(_vsync);
    }

    bool Engine::isVSync() {
        return this->window->isVSyncActive();
    }

    void Engine::setWindowSize(int _width, int _height) {
        this->window->setWindowSize(_width, _height);
    }

    void Engine::pushLayer(Layer* _layer) {
        this->layerStack.pushLayer(_layer);
        _layer->onInit();
    }

    void Engine::pushOverlay(Layer* _layer) {
        this->layerStack.pushOverlay(_layer);
        _layer->onInit();
    }

    void Engine::popLayer(Layer* _layer) {
        this->layerStack.popLayer(_layer);
        _layer->onEnd();
    }

    void Engine::popOverlay(Layer* _layer) {
        this->layerStack.popOverlay(_layer);
        _layer->onEnd();
    }

    void Engine::closeApplication() {
        this->running = false;
    }

    bool Engine::fromRunToRoll(const TransitionParams& _foo) {
        return InputManager::isKeyJustPressed(KeyCode::A);
    }

    bool Engine::fromRollToRun(const TransitionParams& _foo) {
        return InputManager::isKeyJustPressed(KeyCode::S);
    }

    Logs Engine::changeColorConsoleCommand(const std::vector<std::string>& _args) {
        backgroundColor = {(unsigned char)std::stoi(_args[0]), (unsigned char)std::stoi(_args[1]),
                           (unsigned char)std::stoi(_args[2]), (unsigned char)std::stoi(_args[3])};
        return {"Changed color"};
    }

}