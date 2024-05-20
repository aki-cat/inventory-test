#include "ui/ClickableIcon.h"

namespace ui {

ClickableIcon::ClickableIcon(const std::string &texture_path, Vector2 collision_size)
        : sprite(texture_path),
          collision_size(collision_size) {}

Rectangle ClickableIcon::clickable_rect() const {
    return Rectangle{
            sprite.position.x - collision_size.x * 0.5f, sprite.position.y - collision_size.y * 0.5f,
            collision_size.x, collision_size.y
    };
}

void ClickableIcon::draw() const {
    sprite.draw();
    // DrawRectangleRec(clickable_rect(), ColorAlpha(RED, 0.333f));
}

}
