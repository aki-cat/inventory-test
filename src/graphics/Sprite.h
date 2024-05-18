
#ifndef UITEST_SPRITE_H
#define UITEST_SPRITE_H

#include <string>
#include <raylib.h>

namespace graphics {

class Sprite {
public:
    explicit Sprite(const std::string &texture_path);
    ~Sprite();

    void draw(const Vector2 &position, float scale, float rotation) const;
private:
    Texture2D _texture{};
};

}

#endif //UITEST_SPRITE_H
