#include "../../include/SFLib.hpp"
#include <iostream>


/* Canonical form */

SFLib::SFLib() 
: dlPtr(nullptr), window(nullptr), width(800), height(600), title("SFML Window")
, winCreate(nullptr), winDestroy(nullptr), winClear(nullptr)
, winDisplay(nullptr), winIsOpen(nullptr), winClose(nullptr), winPollEvent(nullptr) 
{}

SFLib::~SFLib() {
    if (this->window) {
        winDestroy(this->window);
    }
    if (this->dlPtr) {
        dlclose(this->dlPtr);
    }
}

SFLib& SFLib::operator=(const SFLib& ref) {
    this->window = ref.window;
    this->width = ref.width;
	this->height = ref.height;
	this->title = ref.title;
	this->dlPtr = ref.dlPtr;
	this->winCreate = ref.winCreate;
	this->winDestroy = ref.winDestroy;
	this->winClear = ref.winClear;
	this->winDisplay = ref.winDisplay;
	this->winIsOpen = ref.winIsOpen;
	this->winClose = ref.winClose;
	this->winPollEvent = ref.winPollEvent;
	return *this;
}

SFLib::SFLib(const SFLib& ref) {
	if (this == &ref)
		return;
	*this = ref;
}

/* Classic constructor */

SFLib::SFLib(int width, int height, const std::string title, const std::string path) {
    
	this->title = title;
	this->window = NULL;
	this->width = width;
	this->height = height;
	this->dlPtr = dlopen(path.c_str(), RTLD_LAZY);
    if (dlPtr == nullptr) {
        std::cerr << "Failed to load the library: " << dlerror() << "\nPath:" << path << std::endl;
        exit(1);
    }

    this->winCreate		= (createWindow_t)		dlsym(dlPtr, "createWindowWrapper");
    this->winDestroy	= (destroyWindow_t)		dlsym(dlPtr, "destroyWindowWrapper");
    this->winClear		= (windowClear_t)		dlsym(dlPtr, "windowClearWrapper");
    this->winDisplay	= (windowDisplay_t)		dlsym(dlPtr, "windowDisplayWrapper");
    this->winIsOpen		= (windowIsOpen_t)		dlsym(dlPtr, "windowIsOpenWrapper");
    this->winClose		= (windowClose_t)		dlsym(dlPtr, "windowCloseWrapper");
    this->winPollEvent	= (windowPollEvent_t)	dlsym(dlPtr, "windowPollEventWrapper");

    if (!this->winCreate || !this->winDestroy|| !this->winClear\
		|| !this->winDisplay || !this->winIsOpen || !this->winClose || !this->winPollEvent) {
        std::cerr << "Failed to load symbols: " << dlerror() << std::endl;
        exit(1);
    }
}


/* Wrapper function */

bool SFLib::windowCreate() {
    this->window = this->winCreate(width, height, title.c_str());
    return (this->window != nullptr);
}

void SFLib::clear() {
    this->winClear(this->window);
}

void SFLib::display() {
    this->winDisplay(this->window);
}

bool SFLib::isOpen() {
    return (this->winIsOpen(this->window));
}

void SFLib::processEvents() {
    sf::Event event;

    while (this->winPollEvent(this->window, &event)) {
        if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) {
            this->winClose(this->window);
        }
    }
}

void SFLib::close() {
	if (this->winIsOpen(this->window)) {
        this->winClose(this->window);
    }
}
