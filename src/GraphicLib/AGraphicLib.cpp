#include "../../include/AGraphicLib.hpp"

AGraphicLib::AGraphicLib() 
: dlPtr(nullptr), window(nullptr), width(0), height(0), title("")
, winCreate(nullptr), winClear(nullptr), winDisplay(nullptr), winClose(nullptr)
, winIsOpen(nullptr), winPollEvent(nullptr), libDestructor(nullptr) {}

AGraphicLib& AGraphicLib::operator=(const AGraphicLib& ref) {
	if (this != &ref) {
		dlPtr = ref.dlPtr;
		window = ref.window;
		width = ref.width;
		height = ref.height;
		title = ref.title;
		winCreate = ref.winCreate;
		winClear = ref.winClear;
		winDisplay = ref.winDisplay;
		winIsOpen = ref.winIsOpen;
		winClose = ref.winClose;
		winPollEvent = ref.winPollEvent;
		libDestructor = ref.libDestructor;
	}
	return *this;
}

AGraphicLib::AGraphicLib(const AGraphicLib& ref) {
	*this = ref;
}

AGraphicLib::AGraphicLib(int width, s32 height, const std::string title, const std::string path) {
	this->width = width;
	this->height = height;
	this->title = title;
	this->window = nullptr;
	this->dlPtr = dlopen(path.c_str(), RTLD_LAZY);
	if (!dlPtr) {
		std::cerr << "Error: " << dlerror() << std::endl;
		exit(1);
	}
    this->winCreate		= (createWindowFunc)dlsym(dlPtr, "createWindowWrapper");
    this->winClear		= (voidWinFunc)dlsym(dlPtr, "windowClearWrapper");
    this->winDisplay	= (voidWinFunc)dlsym(dlPtr, "windowDisplayWrapper");
    this->winClose		= (voidWinFunc)dlsym(dlPtr, "windowCloseWrapper");
    this->winIsOpen		= (boolWinFunc)dlsym(dlPtr, "windowIsOpenWrapper");
    this->winPollEvent	= (winFuncPollFunc)dlsym(dlPtr, "windowPollEventWrapper");
	this->winColorTile	= (tileColorFunc)dlsym(dlPtr, "colorTileWrapper");
	if (title == "SDL2") {
		this->libDestructor = (libDestructorFunc)dlsym(dlPtr, "SDL2LibDestructor");
	}

	if (!this->winCreate || !this->winClear || !this->winDisplay 
		|| !this->winIsOpen || !this->winClose || !this->winPollEvent) {
		std::cerr << "Error: " << dlerror() << std::endl;
		exit(1);
	}

}

/* Wrapper function */

bool AGraphicLib::windowCreate() {
    this->window = this->winCreate(width, height, title.c_str());
    return (this->window != nullptr);
}

void AGraphicLib::clear() {
    this->winClear(this->window);
}

void AGraphicLib::display() {
    this->winDisplay(this->window);
}

void AGraphicLib::colorTile(u32 x, u32 y, u8 r, u8 g, u8 b, u8 a) {
	this->winColorTile(this->window, x, y, r, g, b, a);
}

bool AGraphicLib::isOpen() {
    return (this->window  && this->winIsOpen(this->window));
}