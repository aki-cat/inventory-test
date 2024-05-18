#include "Drawable.h"

graphics::Drawable::Drawable(const std::string &texture_path) : _sprite(texture_path) {}

void graphics::Drawable::draw() const {
    _sprite.draw(position, scale, rotation);
}
