#ifndef INVENTORY_TEST_SPRITE_H
#define INVENTORY_TEST_SPRITE_H

#include <string>
#include <raylib.h>

namespace graphics {

class Sprite {
public:
    Texture2D texture{};
    Vector2 position = Vector2{};
    Vector2 offset = Vector2{}; // offset is normalized!!
    float scale = 1.f;
    float rotation = 0.f;

    explicit Sprite(const std::string &texture_path);
    Sprite(const std::string &texture_path, Vector2 offset);

    Sprite(const Sprite &) = delete;
    Sprite(Sprite &) = delete;
    Sprite &operator=(Sprite &) = delete;
    const Sprite &operator=(const Sprite &) = delete;

    ~Sprite();

    void draw() const;
    Vector2 get_offset_position() const;
};

}

#endif //INVENTORY_TEST_SPRITE_H
