#include "../../include/RayLib.hpp"
#include "../../include/Nibbler.hpp"
#include <iostream>
#include "../../include/Color.hpp"

/* Canonical form */

RayLib::RayLib() 
: AGraphicLib()
{}

RayLib::~RayLib() {
	if (this->window) {
		this->close();
	}
	this->libDestructor();
    if (this->dlPtr) {
        dlclose(this->dlPtr);
    }
}

RayLib& RayLib::operator=(const RayLib& ref) {
	AGraphicLib::operator=(ref);
	return (*this);
}

RayLib::RayLib(const RayLib& ref)
: AGraphicLib(ref)
{}

/* Classic constructor */
RayLib::RayLib(int width, s32 height, const std::string title, const std::string path) 
: AGraphicLib(width, height, title, path)
{}


// static void SDL2HandleSnakeMove(Nibbler &ctx, SDL_Event &event) {
// 	Snake &snake = ctx.getSnake();

// 	if (event.key.keysym.sym == SDLK_UP) {
// 		snake.SnakeMove(ctx, UP);
// 	} else if (event.key.keysym.sym == SDLK_DOWN) {
// 		snake.SnakeMove(ctx, DOWN);
// 	} else if (event.key.keysym.sym == SDLK_LEFT) {
// 		snake.SnakeMove(ctx, LEFT);
// 	} else if (event.key.keysym.sym == SDLK_RIGHT) {
// 		snake.SnakeMove(ctx, RIGHT);
// 	}
// }

void RayLib::processEvents(Nibbler &ctx) {
	return ;
	// SDL_Event event;

    // while (this->winPollEvent(this->window, &event)) {
    //     if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
	// 		ctx.setIsRunning(0);
    //     } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_1) {
	// 		// std::cout << "SDL Key 1 pressed, close SDL window" << std::endl;
	// 		ctx.setCurrentLibIdx(SFML_IDX);
	// 		this->close();
	// 	} else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_2) {
	// 		// std::cout << "SDL Key 2 pressed" << std::endl;
	// 		ctx.setCurrentLibIdx(SDL2_IDX);
	// 	} else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_3) {
	// 		std::cout << RED << "SDL Key 3 pressed, NEED TO IMPLEMENT LAST LIB" << RESET << std::endl;
	// 	} else if (event.type == SDL_KEYDOWN) {
	// 		// SDL2HandleSnakeMove(ctx, event);
	// 	}
    // }
}

void RayLib::close() {
	if (this->window) {
		this->winClose(this->window);
		this->window = nullptr;
	}
}