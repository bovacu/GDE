// Created by borja on 6/2/22.


#include "core/render/window/input/WindowInput.h"
#include "core/render/window/event/WindowEvent.h"
#include "core/Engine.h"

namespace GDE {

    void WindowInput::init(Engine* _engine, Window* _window) {
        window = _window;
        engine = _engine;

        UDelegate<void(SDL_Event&, RmlData*)> weDel, wdeDel, wdebeDel, wdefeDel, wwebeDel, wwefeDel;
        weDel.bind<&WindowInput::onWindowEvent>(this);
        wdeDel.bind<&WindowInput::onDestroyApp>(this);
        wdebeDel.bind<&WindowInput::onDidEnterBackground>(this);
        wdefeDel.bind<&WindowInput::onDidEnterForegroundApp>(this);
        wwebeDel.bind<&WindowInput::onWillEnterBackground>(this);
        wwefeDel.bind<&WindowInput::onWillEnterForegroundApp>(this);

        events[(int)SystemEventEnum::WINDOW_EVENT] = weDel;
        events[(int)SystemEventEnum::APP_ON_DESTROY_E] = wdeDel;
        events[(int)SystemEventEnum::APP_DID_ENTER_BACK_E] = wdebeDel;
        events[(int)SystemEventEnum::APP_DID_ENTER_FOREG_E] = wdefeDel;
        events[(int)SystemEventEnum::APP_WILL_ENTER_BACK_E] = wwebeDel;
        events[(int)SystemEventEnum::APP_WILL_ENTER_FOREG] = wwefeDel;

        ignoredEvents = { WINDOW_AUDIO_DEVICE_CONNECTED_E, WINDOW_AUDIO_DEVICE_DISCONNECTED_E };
    }

    void WindowInput::onWindowEvent(SDL_Event& _event, RmlData* _rmlData) {
        switch (_event.window.event) {
            case SDL_WINDOWEVENT_RESIZED: onWindowResize(_event, _rmlData); break;
            case SDL_WINDOWEVENT_FOCUS_GAINED: onWindowGainFocus(_event, _rmlData); break;
            case SDL_WINDOWEVENT_FOCUS_LOST: onWindowLostFocus(_event, _rmlData); break;
            case SDL_WINDOWEVENT_ENTER: onWindowEnter(_event, _rmlData); break;
            case SDL_WINDOWEVENT_LEAVE: onWindowExit(_event, _rmlData); break;
            case SDL_WINDOWEVENT_MOVED: onWindowMoved(_event, _rmlData); break;
            case SDL_WINDOWEVENT_MINIMIZED: onWindowMinimized(_event, _rmlData); break;
            case SDL_WINDOWEVENT_MAXIMIZED: onWindowMaximized(_event, _rmlData); break;
            case SDL_WINDOWEVENT_CLOSE: onQuit(_event, _rmlData); break;
        }
    }

    void WindowInput::onWindowEnter(SDL_Event& _event, RmlData* _rmlData) {

    }

    void WindowInput::onWindowExit(SDL_Event& _event, RmlData* _rmlData) {

    }

    void WindowInput::onWindowGainFocus(SDL_Event& _event, RmlData* _rmlData) {
        window->minimized = false;
    }

    void WindowInput::onWindowLostFocus(SDL_Event& _event, RmlData* _rmlData) {

    }

    void WindowInput::onWindowResize(SDL_Event& _event, RmlData* _rmlData) {
        WindowResizedEvent _rwEvent(_event.window.data1, _event.window.data2);
        window->consumeEvent(_rwEvent);
    }

    void WindowInput::onWindowMoved(SDL_Event& _event, RmlData* _rmlData) {
        WindowMovedEvent _wmEvent(_event.window.data1, _event.window.data2);
        window->setPosition({_event.window.data1, _event.window.data2});
        window->consumeEvent(_wmEvent);
    }

    void WindowInput::onWindowMinimized(SDL_Event& _event, RmlData* _rmlData) {
        WindowMinimizedEvent _e(1);
        window->consumeEvent(_e);
        window->minimized = true;
    }

    void WindowInput::onWindowMaximized(SDL_Event& _event, RmlData* _rmlData) {
        WindowMinimizedEvent _e(0);
        window->consumeEvent(_e);
        window->minimized = false;
    }

    void WindowInput::onQuit(SDL_Event& _event, RmlData* _rmlData) {
        engine->getWindow().stop();
    }

    void WindowInput::onDidEnterForegroundApp(SDL_Event& _event, RmlData* _rmlData) {
        LOG_W("DID ENTER FOREGROUND")
    }

    void WindowInput::onDidEnterBackground(SDL_Event& _event, RmlData* _rmlData) {
        LOG_W("DID ENTER BACKGROUND")
    }

    void WindowInput::onDestroyApp(SDL_Event& _event, RmlData* _rmlData) {
        LOG_I("AUTO TERMINATING!!!!!")
        engine->getWindow().stop();
    }

    void WindowInput::onWillEnterForegroundApp(SDL_Event& _event, RmlData* _rmlData) {
        LOG_W("WILL ENTER FOREGROUND")
        window->minimized = false;
    }

    void WindowInput::onWillEnterBackground(SDL_Event& _event, RmlData* _rmlData) {
        LOG_W("WILL ENTER BACKGROUND")
        window->minimized = true;
    }
}