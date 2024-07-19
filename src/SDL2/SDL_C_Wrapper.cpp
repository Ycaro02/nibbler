#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

extern "C" {
    SDL_Window* createWindowWrapper(unsigned int width, unsigned int height, const char* title) {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return (nullptr);
        }
        SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
        }
        return (window);
    }

    void windowClearWrapper(SDL_Window* window) {
        SDL_Renderer* renderer = SDL_GetRenderer(window);
        if (!renderer) {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (!renderer) {
                std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
                return;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    void windowDisplayWrapper(SDL_Window* window) {
        SDL_Renderer* renderer = SDL_GetRenderer(window);
        if (!renderer) {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (!renderer) {
                std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
                return;
            }
        }
        SDL_RenderPresent(renderer);
    }

    bool windowIsOpenWrapper(SDL_Window* window) {
        return (window != nullptr);
    }

    void windowCloseWrapper(SDL_Window* window) {
        if (window) {
            SDL_DestroyWindow(window);
            SDL_Quit();
        }
    }

    bool windowPollEventWrapper(SDL_Window* window, SDL_Event* event) {
        return (SDL_PollEvent(event));
    }
}
