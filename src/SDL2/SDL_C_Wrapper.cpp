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

    bool windowPollEventWrapper(SDL_Window* window, SDL_Event* event) {
        return (SDL_PollEvent(event));
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
