#include "Drawable.h"

graphics::Drawable::Drawable(const std::string &texture_path) : _sprite(texture_path) {
    size = {static_cast<float>(_sprite.texture.width), static_cast<float>(_sprite.texture.height)};
}

void graphics::Drawable::draw() const {
    _sprite.draw(position, scale, rotation);
}

Rectangle graphics::Drawable::rect() const {
    return Rectangle{
            position.x - size.x * 0.5f, position.y - size.y * 0.5f,
            size.x, size.y
    };
}
