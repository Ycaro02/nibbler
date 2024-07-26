
/**
 * @file SDL_C_Wrapper.cpp
 * @brief Implementation of the SDL2 wrapper in C
*/

/* Basic SDL include */
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

/* TTF include */
#include <SDL2/SDL_ttf.h>

/* C++ include */
#include <iostream>
#include "../../include/short_type.hpp"
#include "../../include/Nibbler.hpp"

extern "C" {

	/**
	 * @brief Create a window with SDL2
	 * @param width The width of the window
	 * @param height The height of the window
	 * @param title The title of the window
	 * @return The window pointer
	*/
    SDL_Window* createWindowWrapper(u32 width ,u32 height, const char* title) {
        SDL_Window		*window = NULL;
		SDL_Renderer	*renderer = NULL;
		static			 bool isInit = false;
		
		if (!isInit) {
			if (SDL_Init(SDL_INIT_VIDEO) != 0) {
				std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
				return (nullptr);
			}
			if (TTF_Init() != 0) {
				std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
				SDL_Quit();
				return (nullptr);
			}
			isInit = true;
		}
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
        if (!window) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
			return (nullptr);
		}
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!renderer) {
			std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
			SDL_DestroyWindow(window);
			return (nullptr);
		}
        return (window);
    }

	/**
	 * @brief Clear the window with SDL2
	 * @param window The window pointers
	*/
    void windowClearWrapper(SDL_Window* window) {
        SDL_Renderer* renderer = SDL_GetRenderer(window);
		if (!renderer) {
			std::cerr << "SDL_GetRenderer Error: " << SDL_GetError() << std::endl;
			return;
		}
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

	/**
	 * @brief Display the window with SDL2
	 * @param window The window pointers
	*/
    void windowDisplayWrapper(SDL_Window* window) {
        SDL_Renderer* renderer = SDL_GetRenderer(window);
		if (!renderer) {
			std::cerr << "SDL_GetRenderer Error: " << SDL_GetError() << std::endl;
			return;
		}
        SDL_RenderPresent(renderer);
    }

    /**
	 * @brief Check if the window is open with SDL2
	 * @param window The window pointers
 	 * @return true if the window is open, false otherwise
	 * @note This function is little bit different from the other lib
	 * 	- SDL2 doesn't have a function to check if the window is open
	 * 	- So we will check if the window ptr is not null
	*/
	bool windowIsOpenWrapper(SDL_Window* window) {
        return (window != nullptr);
    }

	/**
	 * @brief Close the window with SDL2
	 * @param window The window pointers
	 * @note We need to destroy renderer before destroying the window
	 */
    void windowCloseWrapper(SDL_Window* window) {
        SDL_Renderer *renderer = NULL;
		if (window) {
			renderer = SDL_GetRenderer(window);
			if (!renderer) {
				std::cerr << "SDL_GetRenderer Error: " << SDL_GetError() << std::endl;
				return;
			}
			SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
        }
    }

	/**
	 * @brief Poll the event with SDL2
	 * @param window The window pointers
	 * @return The normalized key (Defined in include/GraphicLib.hpp)
	*/
    s32 windowPollEventWrapper(SDL_Window* window) {
        SDL_Event event;

		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
			return (NKEY_ESC);
		} else if (event.type == SDL_KEYDOWN) {
			SDL_Keycode code = event.key.keysym.sym;
			if (code == SDLK_UP) { return (NKEY_UP); }
			else if (code == SDLK_DOWN) { return (NKEY_DOWN); }
			else if (code == SDLK_LEFT) { return (NKEY_LEFT); }
			else if (code == SDLK_RIGHT) { return (NKEY_RIGHT); }
			else if (code == SDLK_1) { return (NKEY_1); }
			else if (code == SDLK_2) { return (NKEY_2); }
			else if (code == SDLK_3) { return (NKEY_3); }
			else if (code == SDLK_a) { return (NKEY_A); }
		}
		return (NKEY_INVALID);
    }

	/**
	 * @brief Draw a color tile with SDL2
	 * @param window The window pointers
	 * @param tilePos, The position of the tile
	 * @param scale The scale of the tile
	 * @note If scale.x/y are equal to TILE_SIZE, we use TILE_SPACING to space the tiles
	*/
	void colorTileWrapper(SDL_Window* window, iVec2 tilePos, iVec2 scale, u32 color) {
		SDL_Rect		tileRect = {0,0,0,0};
		SDL_Renderer	*renderer = NULL;
		s32				pixel_x = 0, pixel_y = 0;
		u8 				r, g, b, a;

		renderer = SDL_GetRenderer(window);
        if (!renderer) {
            std::cerr << "SDL_GetRenderer Error: " << SDL_GetError() << std::endl;
            return;
        }

		UINT32_TO_RGBA(color, r, g, b, a);

        /* Convert tile coordinates to pixel coordinates */
		if (scale.x == TILE_SIZE && scale.y == TILE_SIZE) {
			TILE_POSITION_TO_PIXEL(tilePos, pixel_x, pixel_y);
		} else {
			pixel_x = tilePos.x; pixel_y = tilePos.y;
		}

		tileRect.x = pixel_x; tileRect.y = pixel_y;
		tileRect.w = scale.x; tileRect.h = scale.y;

        /* Set the drawing color and draw the tile */
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_RenderFillRect(renderer, &tileRect);
    }

	/**
	 * @brief Draw a texture tile with SDL2
	 * @param window The window pointers
	 * @param texture The texture pointer
	 * @param tilePos, The position of the tile
	 * @param scale The scale of the tile
	 * @note If the scale is equal to TILE_SIZE, the function will draw the tile at the right position
	*/
	void drawTextureTileWrapper(SDL_Window *window, SDL_Texture *texture, iVec2 tilePos, iVec2 scale) {
		SDL_Renderer *renderer = SDL_GetRenderer(window);
		SDL_Rect 	dstRect;
		s32 		pixel_x, pixel_y;
		
		if (!texture || !renderer) {
        	return;
    	}

		/* Convert tile coordinates to pixel coordinates */
		if (scale.x == TILE_SIZE && scale.y == TILE_SIZE) {
			TILE_POSITION_TO_PIXEL(tilePos, pixel_x, pixel_y);
		} else {
			pixel_x = tilePos.x;
			pixel_y = tilePos.y;
		}

		dstRect.x = pixel_x;
		dstRect.y = pixel_y;
		dstRect.w = scale.x;
		dstRect.h = scale.y;
		SDL_RenderCopy(renderer, texture, NULL, &dstRect);
	}

	/**
	 * @brief Load a texture from a path with SDL2
	 * @param window The window pointers
	 * @param path The path of the texture
	 * @return The texture pointer
	*/
	void *loadTextureWrapper(SDL_Window* window, const char* path) {
		SDL_Renderer	*renderer = NULL;
		SDL_Texture		*texture = NULL;

		renderer = SDL_GetRenderer(window);
		if (!renderer) {
			return (nullptr);
		}
		texture = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(path));
		if (!texture) {
			std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
			return (nullptr);
		}
		return (texture);
	}

	/**
	 * @brief Unload a texture with SDL2
	 * @param texture The texture pointer
	*/
	void unloadTextureWrapper(SDL_Texture *texture) {
		if (!texture) {
			return ;
		}
		SDL_DestroyTexture(texture);
	}

	/**
	 * @brief Destructor for the SDL2 library
	 * @note This function will quit the SDL2 subsystem
	*/
	void libDestructorWrapper() {
		TTF_Quit();
		SDL_Quit();
	}


	/**
	 * @brief Load a font with SDL2
	 * @param path The path of the font
	 * @return The font pointer
	*/
	void *loadFontWrapper(const char *path) {
		TTF_Font *font = TTF_OpenFont(path, 40);
		if (!font) {
			std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
			return (nullptr);
		}
		return (font);
	}

	/**
	 * @brief Unload a font with SDL2
	 * @param font The font pointer
	*/
	void unloadFontWrapper(TTF_Font *font) {
		if (font) {
			TTF_CloseFont(font);
		}
	}

	/**
	 * @brief Write a text with SDL2
	 * @param window The window pointers
	 * @param font The font pointer
	 * @param text The text to write
	 * @param pos The position of the text
	 * @param fontSize The size of the text
	 * @param color The color of the text
	*/
	void writeTextWrapper(SDL_Window *window, TTF_Font *font, char *text, iVec2 pos, u32 fontSize, u32 color) {
		SDL_Renderer	*renderer = NULL;
		SDL_Surface		*textSurface = NULL;
		SDL_Texture		*textTexture = NULL;
		SDL_Rect		textRect = {0,0,0,0};
		u8 				r, g, b, a;

		(void)fontSize;

		renderer = SDL_GetRenderer(window);
		if (!renderer) {
			std::cerr << "SDL_GetRenderer Error: " << SDL_GetError() << std::endl;
			return;
		}

		UINT32_TO_RGBA(color, r, g, b, a);

		textSurface = TTF_RenderText_Solid(font, text, {r, g, b, a});
		if (!textSurface) {
			std::cerr << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
			return;
		}
		textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (!textTexture) {
			std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
			SDL_FreeSurface(textSurface);
			return;
		}
		textRect.x = pos.x;
		textRect.y = pos.y;
		textRect.w = textSurface->w;
		textRect.h = textSurface->h;
		SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
		SDL_DestroyTexture(textTexture);
		SDL_FreeSurface(textSurface);
	}
}
