// Created by borja on 26/12/21.

#include "Renderer.h"

namespace engine {

    SpriteBatch Renderer::batch;

    void Renderer::init(Window* _window) {
        batch.init(_window);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT,  GL_NICEST);
    }

    void Renderer::clear(const Color& _color) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor((float)_color.r / 255.f, (float)_color.g / 255.f, (float)_color.b / 255.f, (float)_color.a / 255.f);
    }

    void Renderer::beginDraw(Camera& _camera) {
        batch.beginDraw(_camera);
    }

    void Renderer::draw(Texture* _texture, const Vec2F& _position, const Vec2F& _size, const Color& _tintColor, float _rotation) {
        batch.draw(
                glm::vec4(_position.x, _position.y, _texture->getSize().x, _texture->getSize().y),
                glm::vec4(0, 0, _size.x, _size.y),
                glm::vec4((float)_tintColor.r / 255.f, (float)_tintColor.g / 255.f, (float)_tintColor.b / 255.f, (float)_tintColor.a / 255.f),
                _texture, _rotation
                );
    }

    void Renderer::drawLine(const Vec2F& _p0, const Vec2F& _p1, const Color& _color) {
        batch.drawLine(_p0, _p1, _color);
    }

    void Renderer::endDraw() {
        batch.flush();
    }

    void Renderer::draw(Sprite& _sprite) {
        batch.draw(_sprite);
    }

    void Renderer::draw(Text& _text) {
        batch.draw(_text);
    }

    void Renderer::beginDebugDraw(Camera& _camera, float _thickness) {
        batch.setDebugLinesThickness(_thickness);
        batch.beginDraw(_camera);
    }

    void Renderer::endDebugDraw() {
        batch.flushDebug();
    }

    void Renderer::drawSquare(const Vec2F& _position, const Vec2F& _size, const Color& _color, float _rotation) {
        batch.drawSquare(_position, _size, _color, _rotation);
    }

    void Renderer::drawShape(Shape& _shape) {
        batch.drawShape(_shape);
    }

    void Renderer::resetDebugInfo() {
        batch.drawCalls = 0;
        batch.totalTriangles = 0;
    }

    int Renderer::getTotalTriangles() {
        return batch.totalTriangles;
    }

    int Renderer::getDrawCalls() {
        return batch.drawCalls;
    }
}