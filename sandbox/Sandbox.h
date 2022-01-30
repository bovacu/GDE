// Created by borja on 22/1/22.


#ifndef ENGINE2_0_SANDBOX_H
#define ENGINE2_0_SANDBOX_H


#include "core/util/Util.h"
#include "core/render/layers/Layer.h"
#include "core/render/Camera.h"
#include "core/render/elements/Sprite.h"
#include "core/systems/animationSystem/AnimationSystem.h"
#include "core/systems/uiSystem/Text.h"

namespace engine {

    class Engine;
    class Sandbox : public Layer {
        private:
            std::vector<Sprite> sprites;
            Sprite player;
            AnimationSystem animationSystem;
            TransitionParams params;
            Text text;
            Engine* engine;

        public:
            void onInit() override;
            void onEvent(Event& _event) override;
            void onUpdate(Delta _dt) override;
            void onFixedUpdate(Delta _dt) override;
            void onRender(Delta _dt) override;
            void onImGuiRender(Delta _dt) override;
            void onEnd() override;
    };

}


#endif //ENGINE2_0_SANDBOX_H