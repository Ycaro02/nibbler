#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
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
		
		if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return (nullptr);
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
		}
		return (NKEY_INVALID);
    }


	/**
	 * @brief Color a tile at x, y with r, g, b, a
	 * @param window The window pointers
	 * @param y,x The position of the tile
	 * @param r,g,b,a The color of the tile
	*/
	void colorTileWrapper(SDL_Window* window, u32 y, u32 x, u8 r, u8 g, u8 b, u8 a) {
		SDL_Rect		tileRect = {0,0,0,0};
		SDL_Renderer	*renderer = NULL;
		s32				pixel_x = 0, pixel_y = 0;

		renderer = SDL_GetRenderer(window);
        if (!renderer) {
            std::cerr << "SDL_GetRenderer Error: " << SDL_GetError() << std::endl;
            return;
        }
        /* Convert tile coordinates to pixel coordinates */
        pixel_x = x * TILE_SIZE + (x + 1) * TILE_SPACING;
        pixel_y = y * TILE_SIZE + (y + 1) * TILE_SPACING;
        tileRect = { pixel_y, pixel_x, TILE_SIZE, TILE_SIZE };
        /* Set the drawing color and draw the tile */
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_RenderFillRect(renderer, &tileRect);
    }

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
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		SDL_Quit();
	}

}
