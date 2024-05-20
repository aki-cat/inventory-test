#include "Sprite.h"

#include <raylib.h>
#include <raymath.h>

namespace graphics {

Sprite::Sprite(const std::string &texture_path) {
    texture = LoadTexture(texture_path.c_str());
}

Sprite::~Sprite() {
    UnloadTexture(texture);
}

void Sprite::draw(const Vector2 &position, float scale, float rotation) const {
    const Vector2 offset_position = get_offset_position(position, scale, rotation);
    DrawTextureEx(texture, offset_position, rotation, scale, WHITE);
    DrawCircle(static_cast<int>(position.x), static_cast<int>(position.y), 2.f, BLUE);
    DrawCircle(static_cast<int>(offset_position.x), static_cast<int>(offset_position.y), 2.f, RED);
    DrawCircle(static_cast<int>(offset_position.x + static_cast<float>(texture.width) * scale),
               static_cast<int>(offset_position.y + static_cast<float>(texture.height) * scale),
               2.f, RED);
}

Vector2 Sprite::get_offset_position(const Vector2 &position, float scale, float rotation) const {
    Vector2 size = {static_cast<float>(texture.width), static_cast<float>(texture.height)};
    Vector2 offset = Vector2Scale(size, .5f);
    Vector2 pivot = Vector2Scale(offset, -1.f);

    pivot = Vector2Scale(pivot, scale);
    pivot = Vector2Rotate(pivot, rotation);
    pivot = Vector2Add(pivot, position);

    return pivot;
}

}
