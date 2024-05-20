#ifndef INVENTORY_TEST_SPRITE_H
#define INVENTORY_TEST_SPRITE_H

#include <string>
#include <raylib.h>

namespace graphics {

class Sprite {
public:
    Texture2D texture{};

    explicit Sprite(const std::string &texture_path);
    ~Sprite();

    void draw(const Vector2 &position, float scale, float rotation) const;
    Vector2 get_offset_position(const Vector2 &position, float scale, float rotation) const;
};

}

#endif //INVENTORY_TEST_SPRITE_H
