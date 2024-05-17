#include <raylib.h>

int main() {
    InitWindow(400, 400, "UITest");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Color{0x20, 0x20, 0x20, 0xff});
        if (IsKeyPressed(KeyboardKey::KEY_F8)) {
            break;
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
