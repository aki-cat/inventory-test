#include "Drawable.h"
#include <raymath.h>

graphics::Drawable::Drawable(const std::string &texture_path) : _sprite(texture_path) {
    offset = Vector2Scale(size(), 0.5f);
}

void graphics::Drawable::draw() const {
    _sprite.draw(Vector2Subtract(position, Vector2Scale(offset, scale)), scale, rotation);
}

Vector2 graphics::Drawable::size() const {
    return Vector2{(float) _sprite.texture.width, (float) _sprite.texture.height};
}

Rectangle graphics::Drawable::rect() const {
    Vector2 root = Vector2Subtract(position, Vector2Scale(offset, scale));
    Vector2 dimensions = Vector2Scale(size(), scale);
    return Rectangle{root.x, root.y, dimensions.x, dimensions.y};
}
