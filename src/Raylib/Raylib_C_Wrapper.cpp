#include "raylib.h"
#include <iostream>
#include <cstring>
#include "../../include/short_type.hpp"
#include "../../include/Nibbler.hpp"

#define RAYLIB_WINPTR ((void *)0x42)

extern "C" {
    void* createWindowWrapper(u32 width, u32 height, const char* title) {
        InitWindow(width, height, title);
        if (!IsWindowReady()) {
            std::cerr << "Failed to initialize Raylib window" << std::endl;
            return nullptr;
        }
        return RAYLIB_WINPTR;
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
            return false;
        }
        return !WindowShouldClose();
    }

    void windowCloseWrapper(void* window) {
        if (window == nullptr || window != RAYLIB_WINPTR) {
            std::cerr << "Invalid window pointer" << std::endl;
            return;
        }
        CloseWindow();
    }

    bool windowPollEventWrapper(void* window, void* event) {
        if (window == nullptr || window != RAYLIB_WINPTR) {
            std::cerr << "Invalid window pointer" << std::endl;
            return false;
        }
		
		/*	
			Need to change wrapper here, instead of bool we will return explicit 
			value for each bind key and adapt the Nibbler class to handle this new event system
		*/

        return true; // Raylib does not have a direct poll event function
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
        DrawRectangle(pixel_x, pixel_y, TILE_SIZE, TILE_SIZE, color);
    }
}
