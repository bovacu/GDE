#include "core/render/window/input/Input.h"
#include "core/Engine.h"
#include "core/render/window/event/JoystickEvent.h"

#include "core/render/window/input/WindowInput.h"
#include "core/render/window/input/KeyboardInput.h"
#include "core/render/window/input/MouseInput.h"
#include "core/render/window/input/ControllerInput.h"
#include "core/render/window/input/MobileInput.h"

namespace engine {

    bool Input::pollEvent(SDL_Event& _event) {
        if(events.find((SystemEventEnum)_event.type) == events.end()) {
            return false;
        }

        events[(int)_event.type](_event);
        return true;
    }

    bool Input::ignoreEvent(const SDL_EventType& _eventType) {
        return std::find(ignoredEvents.begin(), ignoredEvents.end(), _eventType) != ignoredEvents.end();
    }


    InputManager& InputManager::get() {
        static InputManager _inputManager;
        return _inputManager;
    }

    void InputManager::init(Window* _window) {
        windowInput = new WindowInput;
        keyboardInput = new KeyboardInput;
        mouseInput = new MouseInput;
        controllerInput = new ControllerInput;
        mobileInput = new MobileInput;

        windowInput->init(_window);
        keyboardInput->init(_window);
        mouseInput->init(_window);
        controllerInput->init(_window);
        mobileInput->init(_window);
    }

    void InputManager::pollEvents() {
        SDL_Event _event;
        while (SDL_PollEvent(&_event)) {

            bool _eventImplemented = false;

            #if !IS_MOBILE()
            ImGui_ImplSDL2_ProcessEvent(&_event);
            #endif

            _eventImplemented |= windowInput->pollEvent(_event);
            _eventImplemented |= keyboardInput->pollEvent(_event);
            _eventImplemented |= mouseInput->pollEvent(_event);
            _eventImplemented |= controllerInput->pollEvent(_event);
            _eventImplemented |= mobileInput->pollEvent(_event);

            _eventImplemented |= windowInput->ignoreEvent((SDL_EventType)_event.type);
            _eventImplemented |= keyboardInput->ignoreEvent((SDL_EventType)_event.type);
            _eventImplemented |= controllerInput->ignoreEvent((SDL_EventType)_event.type);
            _eventImplemented |= mouseInput->ignoreEvent((SDL_EventType)_event.type);
            _eventImplemented |= mobileInput->ignoreEvent((SDL_EventType)_event.type);

            if(!_eventImplemented) {
                LOG_W("System event ", _event.type, " not implemented!!")
                continue;
            }
        }
    }

    bool InputManager::isKeyJustPressed(KeyCode _key) {
        auto* _keyboardInput = get().keyboardInput;
        if(_keyboardInput->getState((int)_key) == 1) {
            _keyboardInput->setState((int)_key, 2);
            return true;
        }

        return false;
    }

    bool InputManager::isKeyJustReleased(KeyCode _key) {
        auto* _keyboardInput = get().keyboardInput;
        if(_keyboardInput->getState((int)_key) == 0) {
            _keyboardInput->setState((int)_key, 3);
            return true;
        }

        return false;
    }

    bool InputManager::isKeyPressed(KeyCode _key) {
        return get().keyboardInput->getState((int)_key) == 1;
    }

    bool InputManager::isKeyReleased(KeyCode _key) {
        return get().keyboardInput->getState((int)_key) == 0;
    }



    bool InputManager::isMouseJustPressed(MouseCode _mouseButton) {
        auto* _mouseInput = get().mouseInput;
        if(_mouseInput->getState((int)_mouseButton) == 1) {
            _mouseInput->setState((int)_mouseButton, 2);
            return true;
        }

        return false;
    }

    bool InputManager::isMouseJustReleased(MouseCode _button) {
        auto* _mouseInput = get().mouseInput;
        if(_mouseInput->getState((int)_button) == 0) {
            _mouseInput->setState((int)_button, 3);
            return true;
        }

        return false;
    }

    bool InputManager::isMousePressed(MouseCode _button) {
        return get().mouseInput->getState((int)_button) == 1;
    }

    bool InputManager::isMouseReleased(MouseCode _button) {
        return  get().mouseInput->getState((int)_button) == 0;
    }

    Vec2F InputManager::getMousePosScreenCoords(bool _centeredMiddleScreen) {
        auto _mousePos = get().mouseInput->getMousePosition();
        auto& _wi = get().windowInput;
        float _x = _mousePos.x - (_centeredMiddleScreen ? (float)_wi->window->getWindowSize().x / 2.f : 0.f);
        float _y = (_centeredMiddleScreen ? (float)_wi->window->getWindowSize().y / 2.f : 0.f) - _mousePos.y;
        float _zoom = Engine::get().getMainCamera().getCurrentZoomLevel();
        return {_x * _zoom, _y * _zoom};
    }

    Vec2F InputManager::getMousePosWorldPos() {
        auto _mousePos = get().mouseInput->getMousePosition();
        auto& _wi = get().windowInput;
        auto& _camera =Engine::get().getMainCamera();
        float _x = _mousePos.x - (float)_wi->window->getWindowSize().x / 2.f + _camera.getPosition().x;
        float _y = _mousePos.y - (float)_wi->window->getWindowSize().y / 2.f + _camera.getPosition().y;
        float _zoom = _camera.getCurrentZoomLevel();
        return {_x * _zoom, _y * _zoom};
    }



    bool InputManager::reassignController(int _controllerID, int _as) {
        return get().controllerInput->reassignController(_controllerID, _as);
    }

    bool InputManager::isGamepadButtonJustPressed(GamePadButtons _button, int _controllerID) {
        auto* _controllerInput = get().controllerInput;
        if(!_controllerInput->hasController(_controllerID)) return false;

        if(_controllerInput->getButtonState((int)_button, _controllerInput->playerIndexToInnerControllerID(_controllerID)) == 1) {
            _controllerInput->setButtonState((int)_button, 2, _controllerInput->playerIndexToInnerControllerID(_controllerID));
            return true;
        }

        return false;
    }

    bool InputManager::isGamepadButtonJustReleased(GamePadButtons _button, int _controllerID) {
        auto* _controllerInput = get().controllerInput;
        if(!_controllerInput->hasController(_controllerID)) {
            LOG_W("IS FALSE FOR ", _controllerID)
            return false;
        }

        if(_controllerInput->getButtonState((int)_button, _controllerInput->playerIndexToInnerControllerID(_controllerID)) == 0) {
            _controllerInput->setButtonState((int)_button, 3, _controllerInput->playerIndexToInnerControllerID(_controllerID));
            return true;
        }

        return false;
    }

    bool InputManager::isGamepadButtonPressed(GamePadButtons _button, int _controllerID) {
        auto* _controllerInput = get().controllerInput;
        if(!_controllerInput->hasController(_controllerID)) return false;
        return _controllerInput->getButtonState((int)_button, _controllerInput->playerIndexToInnerControllerID(_controllerID)) == 1;
    }

    bool InputManager::isGamepadButtonReleased(GamePadButtons _button, int _controllerID) {
        auto* _controllerInput = get().controllerInput;
        if(!_controllerInput->hasController(_controllerID)) return false;
        return _controllerInput->getButtonState((int)_button, _controllerInput->playerIndexToInnerControllerID(_controllerID)) == 0;
    }

    bool InputManager::isGamepadAxisPressed(GamePadAxis _axis, int _controllerID) {
        auto* _controllerInput = get().controllerInput;
        if(!_controllerInput->hasController(_controllerID)) return false;
        return _controllerInput->getAxisState((int)_axis, _controllerInput->playerIndexToInnerControllerID(_controllerID)) == 0;
    }

    bool InputManager::isGamepadAxisJustPressed(GamePadAxis _axis, int _controllerID) {
        auto* _controllerInput = get().controllerInput;
        if(!_controllerInput->hasController(_controllerID)) return false;
        if(_controllerInput->getAxisState((int)_axis, _controllerInput->playerIndexToInnerControllerID(_controllerID)) == 1) {
            _controllerInput->setAxisState((int)_axis, 2, _controllerInput->playerIndexToInnerControllerID(_controllerID));
            return true;
        }

        return false;
    }

    bool InputManager::isGamepadAxisReleased(GamePadAxis _axis, int _controllerID) {
        auto* _controllerInput = get().controllerInput;
        if(!_controllerInput->hasController(_controllerID)) return false;
        return _controllerInput->getAxisState((int)_axis, _controllerInput->playerIndexToInnerControllerID(_controllerID)) == 0;
    }

    bool InputManager::isMobileScreenJustPressed(int _fingerID) {
        auto* _mobileInput = get().mobileInput;
        if(_mobileInput->getState(_fingerID) == 1) {
            _mobileInput->setState(_fingerID, 2);
            return true;
        }

        return false;
    }

    bool InputManager::isMobileScreenJustReleased(int _fingerID) {
        auto* _mobileInput = get().mobileInput;
        if(_mobileInput->getState(_fingerID) == 0) {
            _mobileInput->setState(_fingerID, 3);
            return true;
        }
        return false;
    }

    bool InputManager::isMobileScreenPressed(int _fingerID) {
        return get().mobileInput->getState(_fingerID) == 1;
    }

    bool InputManager::isMobileScreenUp(int _fingerID) {
        return get().mobileInput->getState(_fingerID) == 0;
    }

    std::vector<SystemEventEnum> InputManager::getEventsIgnored(const InputType& _inputType) {
        switch (_inputType) {
            case WINDOW: return get().windowInput->ignoredEvents;
            case MOUSE: return get().mouseInput->ignoredEvents;
            case KEYBOARD: return get().keyboardInput->ignoredEvents;
            case CONTROLLER: return get().controllerInput->ignoredEvents;
            case MOBILE: return get().mobileInput->ignoredEvents;
        }
    }

    void InputManager::addEventToIgnore(const InputType& _inputType, const SystemEventEnum& _event) {
        switch (_inputType) {
            case WINDOW: {
                auto& _v = get().windowInput->ignoredEvents;
                if(std::find(_v.begin(), _v.end(), _event) == _v.end())
                    get().windowInput->ignoredEvents.push_back(_event);
            }
            case MOUSE: {
                auto& _v = get().mouseInput->ignoredEvents;
                if(std::find(_v.begin(), _v.end(), _event) == _v.end())
                get().mouseInput->ignoredEvents.push_back(_event);
            }
            case KEYBOARD: {
                auto& _v = get().keyboardInput->ignoredEvents;
                if(std::find(_v.begin(), _v.end(), _event) == _v.end())
                    get().keyboardInput->ignoredEvents.push_back(_event);
            }
            case CONTROLLER: {
                auto& _v = get().controllerInput->ignoredEvents;
                if(std::find(_v.begin(), _v.end(), _event) == _v.end())
                    get().controllerInput->ignoredEvents.push_back(_event);
            }
            case MOBILE: {
                auto& _v = get().mobileInput->ignoredEvents;
                if(std::find(_v.begin(), _v.end(), _event) == _v.end())
                    get().mobileInput->ignoredEvents.push_back(_event);
            }
        }
    }

    void InputManager::removeEventToIgnore(const InputType& _inputType, const SystemEventEnum& _event) {
        switch (_inputType) {
            case WINDOW: {
                auto& _v = get().windowInput->ignoredEvents;
                _v.erase(std::remove(_v.begin(), _v.end(), _event), _v.end());
            }
            case MOUSE: {
                auto& _v = get().mouseInput->ignoredEvents;
                _v.erase(std::remove(_v.begin(), _v.end(), _event), _v.end());
            }
            case KEYBOARD: {
                auto& _v = get().keyboardInput->ignoredEvents;
                _v.erase(std::remove(_v.begin(), _v.end(), _event), _v.end());
            }
            case CONTROLLER: {
                auto& _v = get().controllerInput->ignoredEvents;
                _v.erase(std::remove(_v.begin(), _v.end(), _event), _v.end());
            }
            case MOBILE: {
                auto& _v = get().mobileInput->ignoredEvents;
                _v.erase(std::remove(_v.begin(), _v.end(), _event), _v.end());
            }
        }
    }

    void InputManager::destroy() {
        delete windowInput;
        delete mouseInput;
        delete keyboardInput;

        controllerInput->destroy();
        delete controllerInput;
        delete mobileInput;
    }
}