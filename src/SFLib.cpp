#include "../include/SFLib.hpp"
#include <iostream>

SFLib::SFLib() 
: dlPtr(nullptr), window(nullptr), width(800), height(600), title("SFML Window")
, createWindow(nullptr), destroyWindow(nullptr), windowClear(nullptr)
, windowDisplay(nullptr), windowIsOpen(nullptr), windowClose(nullptr), windowPollEvent(nullptr) 
{}

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

    if (!this->createWindow || !this->destroyWindow || !this->windowClear || !this->windowDisplay || !this->windowIsOpen || !this->windowClose || !this->windowPollEvent) {
        std::cerr << "Failed to load symbols: " << dlerror() << std::endl;
        exit(1);
    }
}

SFLib::~SFLib() {
    if (window) {
        destroyWindow(window);
    }
    if (dlPtr) {
        dlclose(dlPtr);
    }
}

SFLib& SFLib::operator=(const SFLib& ref) {
    window = ref.window;
    return *this;
}

bool SFLib::init() {
    window = createWindow(width, height, title.c_str());
    return (window != nullptr);
}

void SFLib::clear() {
    windowClear(window);
}

void SFLib::display() {
    windowDisplay(window);
}

bool SFLib::isOpen() {
    return windowIsOpen(window);
}

void SFLib::processEvents() {
    sf::Event event;
    while (windowPollEvent(window, &event)) {
        if (event.type == sf::Event::Closed) {
            windowClose(window);
        }
    }
}

void SFLib::close() {
    if (windowIsOpen(window)) {
        windowClose(window);
    }
}
