#include "../include/SFLib.hpp"
#include <iostream>

SFLib::SFLib() 
: dlPtr(nullptr), window(nullptr), width(800), height(600), title("SFML Window")
, createWindow(nullptr), destroyWindow(nullptr), windowClear(nullptr)
, windowDisplay(nullptr), windowIsOpen(nullptr), windowClose(nullptr), windowPollEvent(nullptr) 
{}

SFLib::~SFLib() {
    if (this->window) {
        destroyWindow(this->window);
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
	this->createWindow = ref.createWindow;
	this->destroyWindow = ref.destroyWindow;
	this->windowClear = ref.windowClear;
	this->windowDisplay = ref.windowDisplay;
	this->windowIsOpen = ref.windowIsOpen;
	this->windowClose = ref.windowClose;
	this->windowPollEvent = ref.windowPollEvent;
	return *this;
}

SFLib::SFLib(const SFLib& ref) {
	if (this == &ref)
		return;
	*this = ref;
}

SFLib::SFLib(const std::string& path, int width, int height, std::string& title) {
    
	this->title = title;
	this->window = NULL;
	this->width = width;
	this->height = height;
	this->dlPtr = dlopen(path.c_str(), RTLD_LAZY);
    if (dlPtr == nullptr) {
        std::cerr << "Failed to L load the library: " << dlerror() << "\nPath:" << path << std::endl;
        exit(1);
    }

    this->createWindow = reinterpret_cast<createWindow_t>(dlsym(dlPtr, "createWindowWrapper"));
    this->destroyWindow = reinterpret_cast<destroyWindow_t>(dlsym(dlPtr, "destroyWindowWrapper"));
    this->windowClear = reinterpret_cast<windowClear_t>(dlsym(dlPtr, "windowClearWrapper"));
    this->windowDisplay = reinterpret_cast<windowDisplay_t>(dlsym(dlPtr, "windowDisplayWrapper"));
    this->windowIsOpen = reinterpret_cast<windowIsOpen_t>(dlsym(dlPtr, "windowIsOpenWrapper"));
    this->windowClose = reinterpret_cast<windowClose_t>(dlsym(dlPtr, "windowCloseWrapper"));
    this->windowPollEvent = reinterpret_cast<windowPollEvent_t>(dlsym(dlPtr, "windowPollEventWrapper"));

    if (!this->createWindow || !this->destroyWindow || !this->windowClear\
		|| !this->windowDisplay || !this->windowIsOpen || !this->windowClose || !this->windowPollEvent) {
        std::cerr << "Failed to load symbols: " << dlerror() << std::endl;
        exit(1);
    }
}

bool SFLib::init() {
    this->window = this->createWindow(width, height, title.c_str());
    return (this->window != nullptr);
}

void SFLib::clear() {
    this->windowClear(this->window);
}

void SFLib::display() {
    this->windowDisplay(this->window);
}

bool SFLib::isOpen() {
    return (this->windowIsOpen(this->window));
}

void SFLib::processEvents() {
    sf::Event event;

    while (this->windowPollEvent(this->window, &event)) {
        if (event.type == sf::Event::Closed) {
            this->windowClose(this->window);
        }
    }
}

void SFLib::close() {
	if (this->windowIsOpen(this->window)) {
        this->windowClose(this->window);
    }
}
