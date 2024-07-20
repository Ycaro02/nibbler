#include "../../include/SDL2Lib.hpp"
#include <iostream>

/* Canonical form */

SDL2Lib::SDL2Lib() 
: dlPtr(nullptr), window(nullptr), width(800), height(600), title("SFML Window")
, winCreate(nullptr), winClear(nullptr), winDisplay(nullptr)
, winIsOpen(nullptr), winClose(nullptr), winPollEvent(nullptr) 
{}

SDL2Lib::~SDL2Lib() {
    if (this->window) {
		free(this->window);
    }
    if (this->dlPtr) {
        dlclose(this->dlPtr);
    }
}

SDL2Lib& SDL2Lib::operator=(const SDL2Lib &ref) {
    this->window = ref.window;
    this->width = ref.width;
	this->height = ref.height;
	this->title = ref.title;
	this->dlPtr = ref.dlPtr;
	this->winCreate = ref.winCreate;
	this->winClear = ref.winClear;
	this->winDisplay = ref.winDisplay;
	this->winIsOpen = ref.winIsOpen;
	this->winClose = ref.winClose;
	this->winPollEvent = ref.winPollEvent;
	return *this;
}

SDL2Lib::SDL2Lib(const SDL2Lib &ref) {
	if (this == &ref)
		return;
	*this = ref;
}


/* Classic constructor */
SDL2Lib::SDL2Lib(int width, int height, const std::string title, const std::string path) {
    
	this->title = title;
	this->window = NULL;
	this->width = width;
	this->height = height;
	this->dlPtr = dlopen(path.c_str(), RTLD_LAZY);
    if (dlPtr == nullptr) {
        std::cerr << "Failed to load the library: " << dlerror() << "\nPath:" << path << std::endl;
        exit(1);
    }

    this->winCreate		= (createWindow_sdl)	dlsym(dlPtr, "createWindowWrapper");
    this->winClear		= (windowClear_sdl)		dlsym(dlPtr, "windowClearWrapper");
    this->winDisplay	= (windowDisplay_sdl)	dlsym(dlPtr, "windowDisplayWrapper");
    this->winIsOpen		= (windowIsOpen_sdl)	dlsym(dlPtr, "windowIsOpenWrapper");
    this->winClose		= (windowClose_sdl)		dlsym(dlPtr, "windowCloseWrapper");
    this->winPollEvent	= (windowPollEvent_sdl)	dlsym(dlPtr, "windowPollEventWrapper");

    if (!this->winCreate || !this->winClear || !this->winDisplay 
		|| !this->winIsOpen || !this->winClose || !this->winPollEvent) {
        std::cerr << "Failed to load symbols: " << dlerror() << std::endl;
        exit(1);
    }
}

/* Wrapper function */

bool SDL2Lib::windowCreate() {
    this->window = this->winCreate(width, height, title.c_str());
    return (this->window != nullptr);
}

void SDL2Lib::clear() {
    this->winClear(this->window);
}

void SDL2Lib::display() {
    this->winDisplay(this->window);
}

bool SDL2Lib::isOpen() {
    return (this->winIsOpen(this->window));
}

void SDL2Lib::processEvents() {
	SDL_Event event;

    while (this->winPollEvent(this->window, &event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            // this->winClose(this->window);
			this->close();
        }
    }
}

void SDL2Lib::close() {
	if (this->winIsOpen(this->window)) {
        this->winClose(this->window);
    }
}
