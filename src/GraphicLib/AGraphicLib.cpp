#include "../../include/AGraphicLib.hpp"
#include "../../include/Nibbler.hpp"
#include "../../include/Snake.hpp"

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

AGraphicLib::~AGraphicLib() {
	if (window) {
		close();
	}
	if (libDestructor) {
		libDestructor();
	}
	if (dlPtr) {
		dlclose(dlPtr);
	}
}

AGraphicLib::AGraphicLib(int width, s32 height, const std::string title, const std::string path, s16 libraryId) {
	
	this->libID = libraryId;
	this->width = width;
	this->height = height;
	this->title = title;
	this->window = nullptr;
	this->dlPtr = dlopen(path.c_str(), RTLD_LAZY);
	if (!dlPtr) {
		// std::cerr << "Error: dlOpen " << dlerror() << std::endl;
		throw std::invalid_argument("Error: Graphic lib " + path + " not found");
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
		// std::cerr << "Error: dlSym " << dlerror() << std::endl;
		throw std::invalid_argument("Error: Symbole in lib " + path + " not found");
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

static void handleSnakeMove(Nibbler &ctx, s32 event) {
	Snake &snake = ctx.getSnake();

	if (event == KEY_UP) {
		snake.SnakeMove(ctx, UP);
	} else if (event == KEY_DOWN) {
		snake.SnakeMove(ctx, DOWN);
	} else if (event == KEY_LEFT) {
		snake.SnakeMove(ctx, LEFT);
	} else if (event == KEY_RIGHT) {
		snake.SnakeMove(ctx, RIGHT);
	}
}

void AGraphicLib::processEvents(Nibbler &ctx) {
	s32 key = KEY_INVALID;
	
	while ((key = this->winPollEvent(this->window)) != KEY_INVALID) {
		if (key == KEY_ESC) {
			ctx.setIsRunning(0);
			break ;
		} 
		else if (key == KEY_1 || key == KEY_2 || key == KEY_3) {
			if (key != this->libID) {
				std::cout << "Switching to lib " << key << std::endl;
				ctx.setCurrentLibIdx((s32)key);
				this->close();
			}
		} 
		else if (key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT) {
			handleSnakeMove(ctx, key);
		}
		std::cout << "key : " << key << std::endl;
	}
}

void AGraphicLib::close() {
	if (this->window) {
		this->winClose(this->window);
		this->window = nullptr;
	}

}