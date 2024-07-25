#include <raylib.h>
#include <iostream>
#include <cstring>
#include "../../include/short_type.hpp"
#include "../../include/Nibbler.hpp"


/** 
 * Here to maintain compatibility with the other libraries, 
 * Define the window pointer as a constant cause Raylib doesn't provide a way to get the window pointer 
*/
#define RAYLIB_WINPTR ((void *)0x42)

/** 
 * @brief Check if a key is pressed or down
 * @param key The key to check
 * @return true if the key is pressed or down, false otherwise
*/
static bool IsKeyPressedOrDown(int key) {
	return (IsKeyDown(key) || IsKeyPressed(key));
}

/** 
 * @brief Check if the window pointer is valid
 * @param win The window pointer to check
 * @return true if the window pointer is valid, false otherwise
*/
static bool raylibWindowGuard(void *win) {
	if (win == nullptr || win != RAYLIB_WINPTR) {
		return (false);
	}
	return (true);
}

extern "C" {

	/**
	 * @brief Create a window with Raylib
	 * @param width The width of the window
	 * @param height The height of the window
	 * @param title The title of the window
	 * @return The window pointer
	*/
    void* createWindowWrapper(u32 width, u32 height, const char* title) {
        /* Disable raylib logs*/
		SetTraceLogLevel(LOG_NONE);
        InitWindow(width, height, title);
		if (!IsWindowReady()) {
            std::cerr << "Failed to initialize Raylib window" << std::endl;
            return (nullptr);
        }
		SetTargetFPS(60);
        return (RAYLIB_WINPTR);
    }

	/**
	 * @brief Clear the window with Raylib
	 * @param window The window pointers
	*/
    void windowClearWrapper(void* window) {
		if (!raylibWindowGuard(window)) {
			return;
		}
        BeginDrawing();
        ClearBackground(BLACK);
    }

	/**
	 * @brief Display the window with Raylib
	 * @param window The window pointers
	 * @note This function must be called after windowClearWrapper
	*/
    void windowDisplayWrapper(void* window) {
		if (!raylibWindowGuard(window)) {
			return;
		}
        EndDrawing();
    }

	/**
	 * @brief Check if the window is open with Raylib
	 * @param window The window pointers
	 * @return true if the window is open, false otherwise
	*/
    bool windowIsOpenWrapper(void* window) {
		if (!raylibWindowGuard(window)) {
			return (false);
		}
        return (!WindowShouldClose());
    }

	/**
	 * @brief Close the window with Raylib
	 * @param window The window pointers
	*/
    void windowCloseWrapper(void* window) {
		if (!raylibWindowGuard(window)) {
			return;
		}
        CloseWindow();
    }

	/**
	 * @brief Poll the event with Raylib
	 * @param window The window pointers
	 * @return The normalized key (Defined in include/GraphicLib.hpp)
	*/
	s32 windowPollEventWrapper(void* window) {
		if (!raylibWindowGuard(window)) {
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

	/**
	 * @brief Color a tile with Raylib
	 * @param window The window pointers
	 * @param y,x The position of the tile
	 * @param r,g,b,a The color of the tile
	 */
    void colorTileWrapper(void* window, iVec2 tilePos, iVec2 scale, u32 color) {
		Color	rayColor;
		s32		pixel_x, pixel_y;
		u8		r, g, b, a;
		if (!raylibWindowGuard(window)) {
			return;
		}
        /* Convert tile coordinates to pixel coordinates */
		if (scale.x == TILE_SIZE && scale.y == TILE_SIZE) {
			pixel_x = tilePos.x * TILE_SIZE + (tilePos.x + 1) * TILE_SPACING;
			pixel_y = tilePos.y * TILE_SIZE + (tilePos.y + 1) * TILE_SPACING;
		} else {
			pixel_x = tilePos.x;
			pixel_y = tilePos.y;
		}

		UINT32_TO_RGBA(color, r, g, b, a);

        rayColor = { r, g, b, a };
        DrawRectangle(pixel_x, pixel_y, scale.x, scale.y, rayColor);
    }

	void *loadTextureWrapper(void* window, const char* path) {
		Texture2D	*texture = NULL;
		Image		image = {0};
		if (!raylibWindowGuard(window)) {
			return (nullptr);
		}
		image = LoadImage(path);
		if (image.data == NULL) {
			std::cerr << "Failed to load image: " << path << std::endl;
			return (nullptr);
		}
		texture = new Texture2D(LoadTextureFromImage(image));
		UnloadImage(image);
		return (texture);
	}

	void unloadTextureWrapper(Texture2D *texture) {
		if (texture) {
			UnloadTexture(*texture);
			delete texture;
		}
	}

	void drawTextureTileWrapper(void* window, Texture2D *texture, iVec2 tilePos, iVec2 scale) {
		Vector2 position;
		f32		scaleX, scaleY;

		if (!raylibWindowGuard(window)) {
        	return;
    	}
		if (!texture) {	
			std::cout << "Error: Texture is null" << std::endl;
		}
		if (scale.x == TILE_SIZE && scale.y == TILE_SIZE) {
			position.x = static_cast<f32>(tilePos.x * TILE_SIZE + (tilePos.x + 1) * TILE_SPACING);
			position.y = static_cast<f32>(tilePos.y * TILE_SIZE + (tilePos.y + 1) * TILE_SPACING);
		} else {
			position.x = static_cast<f32>(tilePos.x);
			position.y = static_cast<f32>(tilePos.y);
		}
		// Calculate scale factors
		scaleX = static_cast<f32>(scale.x) / texture->width;
		scaleY = static_cast<f32>(scale.y) / texture->height;
    	// Define source and destination rectangles
		Rectangle sourceRec = { 0.0f, 0.0f, static_cast<f32>(texture->width), static_cast<f32>(texture->height) };
		Rectangle destRec = { position.x, position.y, static_cast<f32>(texture->width) * scaleX, static_cast<f32>(texture->height) * scaleY };
		Vector2 origin = { 0.0f, 0.0f };

		// Draw the texture with the calculated scale
		DrawTexturePro(*texture, sourceRec, destRec, origin, 0.0f, WHITE);
	}

	/**
	 * @brief Raylib destructor
	 * @note Nothing to do here to keep compatibility with the SDL2 lib
	*/
	void libDestructorWrapper() {
		/* Nothing todo */
	}
}
