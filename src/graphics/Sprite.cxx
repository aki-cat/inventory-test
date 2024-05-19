
#include "Sprite.h"

#include <raylib.h>

using graphics::Sprite;

Sprite::Sprite(const std::string &texture_path) {
    texture = LoadTexture(texture_path.c_str());
}

Sprite::~Sprite() {
    UnloadTexture(texture);
}

void Sprite::draw(const Vector2 &position, float scale, float rotation) const {
    DrawTextureEx(texture, position, rotation, scale, WHITE);
}
