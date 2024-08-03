/**
 * @file Raylib_C_Wrapper.cpp
 * @brief Implementation of the Raylib wrapper in extern C to avoid name mangling
*/

/* Basic Raylib include */
#include <raylib.h>

/* C++ include */
#include <iostream>
#include <cstring>
#include "../../include/BasicDefine.hpp"
#include "../../include/Nibbler.hpp"


/** 
 * Here to maintain compatibility with the other libraries, 
 * Define the window pointer as a constant cause Raylib doesn't provide a way to get the window pointer 
*/
#define RAYLIB_WINPTR ((void *)0x42)

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

		/* Center the window 0 for the primary monitor (dual screen setup) */
		Vector2 monitor = GetMonitorPosition(0);
		s32 screenWidth = GetMonitorWidth(0);
		s32 screenHeight = GetMonitorHeight(0);
		
		if (screenWidth <= 0 || screenHeight <= 0) {
        	std::cerr << "Failed to get primary monitor dimensions" << std::endl;
        	CloseWindow();
			return (nullptr);
    	}
		SetWindowPosition(monitor.x + (screenWidth - width) / 2, monitor.y + (screenHeight - height) / 2);
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
        if (WindowShouldClose()) { return (NKEY_ESC); }
		else if (IsKeyPressed(KEY_UP)) { return (NKEY_UP); }
		else if (IsKeyPressed(KEY_DOWN)) { return (NKEY_DOWN); }
		else if (IsKeyPressed(KEY_LEFT)) { return (NKEY_LEFT); }
		else if (IsKeyPressed(KEY_RIGHT)) { return (NKEY_RIGHT); }
		else if (IsKeyPressed(KEY_ONE)) { return (NKEY_1); }
		else if (IsKeyPressed(KEY_TWO)) { return (NKEY_2); }
		else if (IsKeyPressed(KEY_THREE)) { return (NKEY_3); }
		else if (IsKeyPressed(KEY_A)) { return (NKEY_A); }
		else if (IsKeyPressed(KEY_P)) { return (NKEY_P); }
		else if (IsKeyPressed(KEY_ENTER)) { return (NKEY_ENTER); }
        return (NKEY_INVALID);
    }

	/**
	 * @brief Color a tile with Raylib
	 * @param window The window pointers
	 * @param y,x The position of the tile
	 * @param color r,g,b,a The color of the tile
	 * @note If scale.x/y are equal to TILE_SIZE, we use TILE_SPACING to space the tiles
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
			TILE_POSITION_TO_PIXEL(tilePos, pixel_x, pixel_y);
		} else {
			pixel_x = tilePos.x;
			pixel_y = tilePos.y;
		}

		UINT32_TO_RGBA(color, r, g, b, a);

        rayColor = { r, g, b, a };
        DrawRectangle(pixel_x, pixel_y, scale.x, scale.y, rayColor);
    }

	/**
	 * @brief Load a texture with Raylib
	 * @param window The window pointers
	 * @param path The path to the texture
	 * @return The texture pointer
	*/
	void *loadTextureWrapper(void* window, const char* path) {
		Texture2D	*texture = NULL;
		Image		image = {0, 0, 0, 0, 0};
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


	/**
	 * @brief Unload a texture with Raylib
	 * @param texture The texture to unload
	*/
	void unloadTextureWrapper(Texture2D *texture) {
		if (texture) {
			UnloadTexture(*texture);
			delete texture;
		}
	}

	/**
	 * @brief Draw a texture tile with Raylib
	 * @param window The window pointers
	 * @param texture The texture to draw
	 * @param tilePos The position of the tile
	 * @param scale The scale of the tile
	 * @note If scale.x/y are equal to TILE_SIZE, we use TILE_SPACING to space the tiles
	*/
	void drawTextureTileWrapper(void* window, Texture2D *texture, iVec2 tilePos, iVec2 scale) {
		
		Rectangle srcRec, destRec;
		Vector2 position;
		f32		scaleX, scaleY;

		if (!raylibWindowGuard(window)) {
        	return;
    	}
		if (!texture) {	
			std::cout << "Error: Texture is null" << std::endl;
		}
		if (scale.x == TILE_SIZE && scale.y == TILE_SIZE) {
			TILE_POSITION_TO_PIXEL(tilePos, position.x, position.y);
		} else {
			position.x = static_cast<f32>(tilePos.x);
			position.y = static_cast<f32>(tilePos.y);
		}
		/* Calculate scale factors */
		scaleX = static_cast<f32>(scale.x) / texture->width;
		scaleY = static_cast<f32>(scale.y) / texture->height;
    	/* Define source and destination rectangles */
		srcRec = { 0.0f, 0.0f, static_cast<f32>(texture->width), static_cast<f32>(texture->height) };
		destRec = { position.x, position.y, static_cast<f32>(texture->width) * scaleX, static_cast<f32>(texture->height) * scaleY };

		/* Draw the texture with the calculated scale */
		DrawTexturePro(*texture, srcRec, destRec, {0.0f, 0.0f}, 0.0f, WHITE);
	}

	/**
	 * @brief Raylib destructor
	 * @note Nothing to do here to keep compatibility with the SDL2 lib
	*/
	void libDestructorWrapper() {
		/* Nothing todo */
	}

	/**
	 * @brief Load a font with Raylib
	 * @param path The path to the font
	 * @return The font pointer
	*/
	void *loadFontWrapper(const char* path, s32 fontSize) {
		Font *font = new Font();

		(void)fontSize;
		 /* If load loading fail, raylib return default font instead */
		*font = LoadFont(path);
		return (font);
	}

	/**
	 * @brief Unload a font with Raylib
	 * @param font The font to unload
	*/
	void unloadFontWrapper(Font *font) {
		if (font) {
			UnloadFont(*font);
			delete font;
		}
	}

	void writeTextWrapper(void* window, Font *font, char* text, iVec2 pos, u32 fontSize, u32 color) {
		if (!raylibWindowGuard(window) || !font) {
			return;
		}
		u8 r, g, b, a;
		UINT32_TO_RGBA(color, r, g, b, a);

		DrawTextEx(*font, text, {static_cast<f32>(pos.x), static_cast<f32>(pos.y)}, static_cast<f32>(fontSize), 0, {r, g, b, a});
		// DrawText(text, pos.x, pos.y, fontSize, color);
	}
}
