#ifndef INVENTORYTEST_CLICKABLEICON_H
#define INVENTORYTEST_CLICKABLEICON_H

#include "graphics/Sprite.h"

#include <raylib.h>

namespace ui {

class ClickableIcon {
public:
    graphics::Sprite icon;
    Vector2 collision_size;

    explicit ClickableIcon(const std::string &texture_path, Vector2 collision_size);
    Rectangle clickable_rect() const;
    void draw() const;
};

}

#endif //INVENTORYTEST_CLICKABLEICON_H
