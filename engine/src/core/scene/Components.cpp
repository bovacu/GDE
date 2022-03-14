// Created by borja on 11/3/22.

#include "core/scene/Components.h"
#include "core/Engine.h"
#include "glm/ext/quaternion_float.hpp"

namespace engine {


    void Transform::setPosition(const Vec2F& _position) {
        Vec2F _distance = _position - getPosition();

        transformMatrix[3] = transformPosition(_position);

        auto _curr = scene->getComponent<Hierarchy>(id);
        if(_curr->children == 0) return;
        traverseChildrenPosition(_distance, _curr->firstChild);
    }

    void Transform::setX(float _x) {
        setPosition({_x, getY()});
    }

    void Transform::setY(float _y) {
        setPosition({getX(), _y});
    }

    glm::vec4 Transform::transformPosition(const Vec2F& _position) {
        auto _scalingFactor = Engine::get().getMainCamera().getViewport()->getScalingFactor();
        auto _pos = Util::worldToScreenCoords({_position.x * _scalingFactor.x, _position.y * _scalingFactor.y}, &Engine::get().getWindow(), Engine::get().getMainCamera().getAspectRatio());
        auto _translationMat = glm::translate(glm::mat4(1.f), glm::vec3 {1, 1, 1.f});
        return _translationMat[3] * glm::vec4 {_pos.x, _pos.y, 1.f, 1.f};
    }

    void Transform::traverseChildrenPosition(const Vec2F& _distance, const NodeID& _node) {
        if(_node == NODE_ID_NULL) return;
        auto* _nodeHierarchy = scene->getComponent<Hierarchy>(_node);

        size_t _childrenCount = _nodeHierarchy->children;
        for(size_t _child = 0; _child < _childrenCount; _child++) {
            traverseChildrenPosition(_distance, _nodeHierarchy->firstChild);
        }

        auto* _t = scene->getComponent<Transform>(_node);
        auto _position = _t->getPosition() + _distance;
        _t->transformMatrix[3] = transformPosition(_position);

        if(_nodeHierarchy->nextBrother != NODE_ID_NULL)
            traverseChildrenPosition(_distance, _nodeHierarchy->nextBrother);
    }

    Vec2F Transform::getPosition() const {
        return Util::screenToWorldCoords({transformMatrix[3][0], transformMatrix[3][1]}, &Engine::get().getWindow(), Engine::get().getMainCamera().getAspectRatio());
    }


    void Transform::setRotation(float _rotation)  {
        float _difference = _rotation - getRotation();
        rotation = _rotation;
        transformMatrix = glm::rotate(transformMatrix, glm::radians(_difference), { 0.0f, 0.0f, 1.0f });

        auto _curr = scene->getComponent<Hierarchy>(id);
        if(_curr->children == 0) return;
        traverseChildrenRotation(_difference, _curr->firstChild, id);
    }

    void Transform::traverseChildrenRotation(float _difference, const NodeID& _node, const NodeID& _parent) {
        if(_node == NODE_ID_NULL) return;
        auto* _nodeHierarchy = scene->getComponent<Hierarchy>(_node);

        size_t _childrenCount = _nodeHierarchy->children;
        for(size_t _child = 0; _child < _childrenCount; _child++) {
            traverseChildrenRotation(_difference, _nodeHierarchy->firstChild, _parent);
        }

        auto* _t = scene->getComponent<Transform>(_node);
        _t->rotation += _difference;
        auto _parentHierarchy = scene->getComponent<Transform>(_parent);

        auto _cos = std::cos(glm::radians(_difference));
        auto _sin = std::sin(glm::radians(_difference));

        auto _translatedX = _t->getX() - _parentHierarchy->getX();
        auto _translatedY = _t->getY() - _parentHierarchy->getY();

        auto result_x = _translatedX * _cos - _translatedY * _sin;
        auto result_y = _translatedX * _sin + _translatedY * _cos;

        result_x += _parentHierarchy->getX();
        result_y += _parentHierarchy->getY();
        _t->transformMatrix[3] = transformPosition({result_x, result_y});
        _t->transformMatrix = glm::rotate(_t->transformMatrix, glm::radians(_difference), { 0.0f, 0.0f, 1.0f });

        if(_nodeHierarchy->nextBrother != NODE_ID_NULL)
            traverseChildrenRotation(_difference, _nodeHierarchy->nextBrother, _parent);
    }



    void Transform::setScale(const Vec2F& _scale) {
        transformMatrix = glm::scale(transformMatrix, glm::vec3 {getScale().x / _scale.x, getScale().y / _scale.y, 1.f});
        auto _curr = scene->getComponent<Hierarchy>(id);
        if(_curr->children == 0) return;
        traverseChildrenScale(_scale, _curr->firstChild);
    }

    void Transform::traverseChildrenScale(const Vec2F& _scale, const NodeID& _node) {
        if(_node == NODE_ID_NULL) return;
        auto* _nodeHierarchy = scene->getComponent<Hierarchy>(_node);

        size_t _childrenCount = _nodeHierarchy->children;
        for(size_t _child = 0; _child < _childrenCount; _child++) {
            traverseChildrenScale(_scale, _nodeHierarchy->firstChild);
        }

        auto* _t = scene->getComponent<Transform>(_node);
        _t->transformMatrix = glm::scale(_t->transformMatrix, glm::vec3 { _t->getScale().x / _scale.x, _t->getScale().y / _scale.y, 1.f });

        if(_nodeHierarchy->nextBrother != NODE_ID_NULL)
            traverseChildrenScale(_scale, _nodeHierarchy->nextBrother);
    }

}
