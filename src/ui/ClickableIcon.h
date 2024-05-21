#ifndef INVENTORYTEST_CLICKABLEICON_H
#define INVENTORYTEST_CLICKABLEICON_H

#include "graphics/Sprite.h"

#include <raylib.h>

namespace ui {

class ClickableIcon {
public:
    graphics::Sprite sprite;
    Vector2 collision_size;
    bool moved = false;

    explicit ClickableIcon(const std::string &texture_path, Vector2 collision_size);
    ClickableIcon(const ClickableIcon &) = delete;
    ClickableIcon(ClickableIcon &) = delete;
    ClickableIcon &operator=(ClickableIcon &) = delete;
    const ClickableIcon &operator=(const ClickableIcon &) = delete;

    Rectangle clickable_rect() const;
    void draw() const;
};

}

#endif //INVENTORYTEST_CLICKABLEICON_H
