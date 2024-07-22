#include "../../include/GraphicLib.hpp"
#include "../../include/Nibbler.hpp"
#include "../../include/Snake.hpp"

GraphicLib::GraphicLib() 
: dlPtr(nullptr), window(nullptr), width(0), height(0), title("")
, winCreate(nullptr), winClear(nullptr), winDisplay(nullptr), winClose(nullptr)
, winIsOpen(nullptr), winPollEvent(nullptr), libDestructor(nullptr) {}

GraphicLib& GraphicLib::operator=(const GraphicLib& ref) {
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

GraphicLib::GraphicLib(const GraphicLib& ref) {
	*this = ref;
}

GraphicLib::~GraphicLib() {
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

GraphicLib::GraphicLib(int width, s32 height, const std::string title, const std::string path, s16 libraryId) {
	
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

bool GraphicLib::windowCreate() {
    this->window = this->winCreate(width, height, title.c_str());
    return (this->window != nullptr);
}

void GraphicLib::clear() {
    this->winClear(this->window);
}

void GraphicLib::display() {
    this->winDisplay(this->window);
}

void GraphicLib::colorTile(u32 x, u32 y, u8 r, u8 g, u8 b, u8 a) {
	this->winColorTile(this->window, x, y, r, g, b, a);
}

bool GraphicLib::isOpen() {
    return (this->window  && this->winIsOpen(this->window));
}

static void handleSnakeMove(Nibbler &ctx, s32 event) {
	Snake &snake = ctx.getSnake();

	if (event == NKEY_UP) {
		snake.SnakeMove(ctx, UP);
	} else if (event == NKEY_DOWN) {
		snake.SnakeMove(ctx, DOWN);
	} else if (event == NKEY_LEFT) {
		snake.SnakeMove(ctx, LEFT);
	} else if (event == NKEY_RIGHT) {
		snake.SnakeMove(ctx, RIGHT);
	}
}

void GraphicLib::processEvents(Nibbler &ctx) {
	s32 key = NKEY_INVALID;
	
	while ((key = this->winPollEvent(this->window)) != NKEY_INVALID) {
		if (key == NKEY_ESC) {
			ctx.setIsRunning(0);
			break ;
		} 
		else if (key == NKEY_1 || key == NKEY_2 || key == NKEY_3) {
			if (key != this->libID) {
				std::cout << "Switching to lib " << key << std::endl;
				ctx.setCurrentLibIdx((s32)key);
				this->close();
				break ;
			}
		} 
		else if (key == NKEY_UP || key == NKEY_DOWN || key == NKEY_LEFT || key == NKEY_RIGHT) {
			handleSnakeMove(ctx, key);
			break ;
		}
		std::cout << "key : " << key << std::endl;
	}
}

void GraphicLib::close() {
	if (this->window) {
		this->winClose(this->window);
		this->window = nullptr;
	}

}