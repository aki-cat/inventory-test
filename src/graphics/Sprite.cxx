#include "Sprite.h"

#include <raylib.h>
#include <raymath.h>

namespace graphics {

Sprite::Sprite(const std::string &texture_path) : Sprite(texture_path, {.5f, .5f}) {}

Sprite::Sprite(const std::string &texture_path, Vector2 offset) : offset(offset) {
    texture = LoadTexture(("assets/" + texture_path).c_str());
}

Sprite::~Sprite() {
    UnloadTexture(texture);
}

void Sprite::draw() const {
    const Vector2 offset_position = get_offset_position();
    DrawTextureEx(texture, offset_position, rotation, scale, WHITE);
    // DrawCircle(static_cast<int>(position.x), static_cast<int>(position.y), 2.f, BLUE);
    // DrawCircle(static_cast<int>(offset_position.x), static_cast<int>(offset_position.y), 2.f, RED);
    // DrawCircle(static_cast<int>(offset_position.x + static_cast<float>(texture.width) * scale),
    //            static_cast<int>(offset_position.y + static_cast<float>(texture.height) * scale),
    //            2.f, RED);
}

Vector2 Sprite::get_offset_position() const {
    Vector2 size = {static_cast<float>(texture.width), static_cast<float>(texture.height)};
    Vector2 pivot = Vector2Multiply(offset, size);
    pivot = Vector2Scale(pivot, -1.f);

    pivot = Vector2Scale(pivot, scale);
    pivot = Vector2Rotate(pivot, rotation);
    pivot = Vector2Add(pivot, position);

    return pivot;
}

}
