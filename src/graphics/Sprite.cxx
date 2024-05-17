//
// Created by coffeecat on 16/05/24.
//

#include "Sprite.h"

#include <raylib.h>

using graphics::Sprite;

Sprite::Sprite(const char *path) : _texture() {
    _texture = LoadTexture(path);
}

Sprite::~Sprite() {
    UnloadTexture(_texture);
}

void Sprite::draw(int x, int y) const {
    DrawTexture(_texture, x, y, WHITE);
}
