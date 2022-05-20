#pragma once

#ifndef ENGINE_SCENE_H
#define ENGINE_SCENE_H

#include <yaml-cpp/node/node.h>
#include "core/util/Delta.h"
#include "core/graph/Graph.h"
#include "core/graph/Components.h"

namespace GDE {

    class Camera;
    class Window;
    class Engine;
    class Scene {
        friend class Graph;

        private:
            Graph mainGraph;
            std::vector<Camera*> cameras;
            Camera* mainCamera = nullptr;

        protected:
            std::string debugName;
            Engine* engine = nullptr;

        public:
            explicit Scene(Engine* _engine, const std::string& _debugName = "Scene");
            virtual ~Scene() {  };

            /// This function is called when loading the scene from the configuration, it is used to make a custom loading
            /// of your custom components for the internal ECS system.
            /// @param _manager gives access to all of the managers of the engine to load anything needed.
            /// @param _window the window of the application
            /// @param _node the configuration of the .yaml file used to load the scene
            virtual void preInit(Manager* _manager, Window* _window, YAML::Node& _node) {  }

            /// This function is called only once and when the SceneManager/displayScene is invoked. It is used to
            /// initialized any needed values of the scene.
            virtual void onInit() {  }

            /// This function captures all the events that happen inside the application. It captures too mouse, keyboard
            /// and gamepads, but it is not recommended to use this input inside this function. The class InputManager is
            /// the one you are looking for. This is used to manage window related or screen related events mainly.
            /// @param _event the event that was just captured.
            /// @see InputManager
            virtual void onEvent(Event& _event);

            /// This function is executed every frame of the application and here we should update logic and take inputs
            /// to update the logic and components.
            /// @param the time that passed between the current frame and the last one.
            virtual void onUpdate(Delta _dt);

            /// This function is called a fixed amount of times per second. By default it is 60 times per second, but it
            /// can be modified with Engine/setFixedDelta. This loop is used to handle physics and things that need a
            /// consistent frame rate to work properly.
            /// @param _dt fixed value initially set at 1/60
            virtual void onFixedUpdate(Delta _dt);

            /// This function is also called every frame and renders the geometry of the game. It is not virtual because
            /// the main rendering pipeline is handled inside the core of the engine. Users don't need to mess with it.
            /// @param the time that passed between the current frame and the last one.
            void onRender(Delta _dt);

            /// This function is used to render specifically ImGui elements, nothing else.
            /// @param the time that passed between the current frame and the last one.
            virtual void onImGuiRender(Delta _dt) {  }

            /// This function is used to draw debugging lines, squares, circles...
            /// @param the time that passed between the current frame and the last one.
            virtual void onDebugRender(Delta _dt);

            /// This function is called when unloading the scene and should be used to release any allocated memory or
            /// resources.
            virtual void onEnd() {  }

            /// This function returns the graph system of this specific scene, so we can access any entity and it's components,
            /// create new ones or destroy them.
            /// @return the graph of the scene.
            Graph* getMainGraph();

            /// @return all of the cameras used in the scene.
            std::vector<Camera*>& getCameras();

            /// @return of all of the cameras in the scene, the primary one.
            Camera* getMainCamera();

            /// Adds a new camera to the scene and returns it to make any modifications needed.
            Camera* addCamera(Window* window);

            /// Activates or deactivates any given camera. If it is disabled, it won't render anything.
            void enableCamera(const NodeID& _cameraID, bool _enable);

            /// Destroys a specific camera of the scene.
            void removeCamera(const NodeID& _cameraID);

            /// Sets an specific camera as the main one.
            void switchMainCamera(const NodeID& _camera);

            /// Returns the name of the scene.
            [[nodiscard]] const std::string& getName() const { return debugName; }
    };

}

#endif //ENGINE_SCENE_H
