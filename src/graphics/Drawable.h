#ifndef INVENTORY_TEST_DRAWABLE_H
#define INVENTORY_TEST_DRAWABLE_H

#include "graphics/Sprite.h"

#include <string>

namespace graphics {

class Drawable {
public:
    Vector2 position = Vector2{};
    Vector2 size = Vector2{};
    float scale = 1.f;
    float rotation = 0.f;

    explicit Drawable(const std::string &);

    void draw() const;
    Rectangle rect() const;
private:
    Sprite _sprite;
};

}

#endif //INVENTORY_TEST_DRAWABLE_H
