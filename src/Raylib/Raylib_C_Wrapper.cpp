#include <raylib.h>
#include <iostream>
#include <cstring>
#include "../../include/short_type.hpp"
#include "../../include/Nibbler.hpp"


// Define the window pointer as a constant cause Raylib doesn't provide a way to get the window pointer
#define RAYLIB_WINPTR ((void *)0x42)

static bool IsKeyPressedOrDown(int key) {
	return (IsKeyDown(key) || IsKeyPressed(key));
}

extern "C" {
    void* createWindowWrapper(u32 width, u32 height, const char* title) {
        SetTraceLogLevel(LOG_NONE); // Disable raylib logs
        InitWindow(width, height, title);
		if (!IsWindowReady()) {
            std::cerr << "Failed to initialize Raylib window" << std::endl;
            return (nullptr);
        }
		SetTargetFPS(60);
        return (RAYLIB_WINPTR);
    }

    void windowClearWrapper(void* window) {
        if (window == nullptr || window != RAYLIB_WINPTR) {
            std::cerr << "Invalid window pointer" << std::endl;
            return;
        }
        BeginDrawing();
        ClearBackground(BLACK);
    }

    void windowDisplayWrapper(void* window) {
        if (window == nullptr || window != RAYLIB_WINPTR) {
            std::cerr << "Invalid window pointer" << std::endl;
            return;
        }
        EndDrawing();
    }

    bool windowIsOpenWrapper(void* window) {
        if (window == nullptr || window != RAYLIB_WINPTR) {
            return (false);
        }
        return (!WindowShouldClose());
    }

    void windowCloseWrapper(void* window) {
        if (window == nullptr || window != RAYLIB_WINPTR) {
            std::cerr  << "windowCloseWrapper Invalid window pointer" << std::endl;
            return;
        }
        CloseWindow();
    }


	s32 windowPollEventWrapper(void* window) {
        if (window == nullptr || window != RAYLIB_WINPTR) {
            std::cerr << "windowPollEventWrapper Invalid window pointer" << std::endl;
            return (NKEY_INVALID);
        }

        if (WindowShouldClose()) {
            return (NKEY_ESC);
        } else if (IsKeyPressedOrDown(KEY_UP)) {
            return (NKEY_UP);
        } else if (IsKeyPressedOrDown(KEY_DOWN)) {
            return (NKEY_DOWN);
        } else if (IsKeyPressedOrDown(KEY_LEFT)) {
            return (NKEY_LEFT);
        } else if (IsKeyPressedOrDown(KEY_RIGHT)) {
            return (NKEY_RIGHT);
        } else if (IsKeyPressedOrDown(KEY_ONE)) {
            return (NKEY_1);
        } else if (IsKeyPressedOrDown(KEY_TWO)) {
            return (NKEY_2);
        } else if (IsKeyPressedOrDown(KEY_THREE)) {
            return (NKEY_3);
        }
        return (NKEY_INVALID);
    }

    void RaylibDestructor() {
        CloseWindow();
    }

    void colorTileWrapper(void* window, u32 y, u32 x, u8 r, u8 g, u8 b, u8 a) {
        if (window == nullptr || window != RAYLIB_WINPTR) {
            std::cerr << "Invalid window pointer" << std::endl;
            return;
        }

        // Convert tile coordinates to pixel coordinates
        s32 pixel_x = x * TILE_SIZE + (x + 1) * TILE_SPACING;
        s32 pixel_y = y * TILE_SIZE + (y + 1) * TILE_SPACING;

        // Set the drawing color and draw the tile
        Color color = { r, g, b, a };
        DrawRectangle(pixel_y, pixel_x, TILE_SIZE, TILE_SIZE, color);
    }
}
