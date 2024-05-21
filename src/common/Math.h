#ifndef INVENTORYTEST_MATH_H
#define INVENTORYTEST_MATH_H

#include <raylib.h>

namespace common {

inline bool is_point_in_rect(Vector2 point, Rectangle rect) {
    return !(point.x < rect.x || point.x > rect.x + rect.width ||
             point.y < rect.y || point.y > rect.y + rect.height);
}

}

#endif //INVENTORYTEST_MATH_H
