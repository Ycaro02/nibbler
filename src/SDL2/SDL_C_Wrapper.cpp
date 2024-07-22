#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <iostream>
#include "../../include/short_type.hpp"
#include "../../include/Nibbler.hpp"

extern "C" {
    SDL_Window* createWindowWrapper(u32 width ,u32 height, const char* title) {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return (nullptr);
        }
        SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
        }
		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!renderer) {
			std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
			SDL_DestroyWindow(window);
            SDL_Quit();
			return (nullptr);
		}
        return (window);
    }

    void windowClearWrapper(SDL_Window* window) {
        SDL_Renderer* renderer = SDL_GetRenderer(window);
		if (!renderer) {
			std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
			return;
		}
        // SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    void windowDisplayWrapper(SDL_Window* window) {
        SDL_Renderer* renderer = SDL_GetRenderer(window);
		if (!renderer) {
			std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
			return;
		}
        SDL_RenderPresent(renderer);
    }

    bool windowIsOpenWrapper(SDL_Window* window) {
        return (window != nullptr);
    }

    void windowCloseWrapper(SDL_Window* window) {
        if (window) {
			SDL_Renderer* renderer = SDL_GetRenderer(window);
			if (!renderer) {
				std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
				return;
			}
			SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
        }
    }

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

	void SDL2LibDestructor() {
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		SDL_Quit();
	}


	void colorTileWrapper(SDL_Window* window, u32 y, u32 x, u8 r, u8 g, u8 b, u8 a) {
        SDL_Renderer* renderer = SDL_GetRenderer(window);
        if (!renderer) {
            std::cerr << "SDL_GetRenderer Error: " << SDL_GetError() << std::endl;
            return;
        }

        // Convert tile coordinates to pixel coordinates
        s32 pixel_x = x * TILE_SIZE + (x + 1) * TILE_SPACING;
        s32 pixel_y = y * TILE_SIZE + (y + 1) * TILE_SPACING;

        SDL_Rect tileRect = { pixel_y, pixel_x, TILE_SIZE, TILE_SIZE };

        // Set the drawing color and draw the tile
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_RenderFillRect(renderer, &tileRect);
    }
}
