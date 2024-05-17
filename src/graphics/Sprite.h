//
// Created by coffeecat on 16/05/24.
//

#ifndef UITEST_SPRITE_H
#define UITEST_SPRITE_H

#include <raylib.h>

namespace graphics {
    class Sprite {
    public:
        explicit Sprite(const char *path);

        ~Sprite();

        void draw(int x, int y) const;

    private:
        Texture2D _texture;
    };
}


#endif //UITEST_SPRITE_H
