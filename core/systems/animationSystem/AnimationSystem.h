// Created by borja on 9/1/22.


#ifndef ENGINE2_0_ANIMATION_SYSTEM_H
#define ENGINE2_0_ANIMATION_SYSTEM_H


#include "core/util/Util.h"
#include "Animation.h"
#include "core/systems/eventSystem/EventBus.h"
#include <functional>

namespace engine {

    struct AnimationNode;
    struct AnimationTransition {
        std::string transitionCode;
        AnimationNode* nextAnimation;
    };

    struct AnimationNode {
        Animation animation;
        std::vector<AnimationTransition> transitions;
    };

    class TransitionParams {
        // when assigning this, it must be the reference to that variable (&)
        private:
            void* whatEver = nullptr;

        public:
            template<typename T>
            void setValue(T& _value) {
                whatEver = &_value;
            }

            template<typename T>
            T& getValue() {
                ENGINE_ASSERT(whatEver, nullptr)
                return *(T*)whatEver;
            }
    };

    typedef std::function<bool(const TransitionParams&)> transitionFunc;

    class AnimationSystem {
        private:
            std::unordered_map<std::string, AnimationNode> animations;
            AnimationNode* currentNode;
            EventBus<std::string, TransitionParams> bus;

        public:
            Animation* createAnimation(const std::string& _animName, const std::string& _atlas, const std::vector<int>& _indices);

            void setAnimationTimeBetweenFrames(const std::string& _animation, float _timeBetweenFrames);
            float getAnimationTimeBetweenFrames(const std::string& _animation);

            void setInitialAnimation(const std::string& _animationName);
            void createTransition(const std::string& _initialAnimation, const std::string& _finalAnimation, transitionFunc _condition);

            AnimationNode* getCurrentAnimation();

            void start();
            void pause();
            void stop();
            void restart();

            void update(float _dt, TransitionParams _params = {});
    };

}


#endif //ENGINE2_0_ANIMATION_SYSTEM_H