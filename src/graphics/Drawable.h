#ifndef INVENTORYTEST_DRAWABLE_H
#define INVENTORYTEST_DRAWABLE_H

#include "graphics/Sprite.h"

#include <string>

namespace graphics {

class Drawable {
public:
    Vector2 position = Vector2{};
    float scale = 1.f;
    float rotation = 0.f;

    explicit Drawable(const std::string &);

    void draw() const;
private:
    Sprite _sprite;
};

}

#endif //INVENTORYTEST_DRAWABLE_H
