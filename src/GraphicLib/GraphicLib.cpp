#include "../../include/GraphicLib.hpp"
#include "../../include/Nibbler.hpp"
#include "../../include/Snake.hpp"
#include "../../include/Color.hpp"

/* Default constructor */
GraphicLib::GraphicLib() 
: dlPtr(nullptr), window(nullptr), width(0), height(0), title("")
, winCreate(nullptr), winClear(nullptr), winDisplay(nullptr), winClose(nullptr)
, winIsOpen(nullptr), winPollEvent(nullptr), libDestructor(nullptr) {}

/* Assignment operator */
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
	return (*this);
}

/* Copy constructor */
GraphicLib::GraphicLib(const GraphicLib& ref) {
	*this = ref;
}

/* Destructor */
GraphicLib::~GraphicLib() {

	std::string name = libID == SDL2_IDX ? "SDL2" : libID == SFML_IDX ? "SFML" : "Raylib";

	std::cout << YELLOW << "GraphicLib Destructor for " + name + " :";
	if (window) {
		std::cout << " with window ";
		close();
	}
	if (libDestructor) {
		std::cout << " with libDestructor ";
		libDestructor();
	}
	if (dlPtr) {
		std::cout << " with dlclose" << RESET << std::endl;
		dlclose(dlPtr);
	}
}

/**	@brief Graphic lib Constructor 
 *	@param width width of the window
 *	@param height height of the window
 *	@param title title of the window
 *	@param path path to the dynamic library
 *	@param libraryId id of the library (SDL2, SFML, Raylib)
*/
GraphicLib::GraphicLib(s32 width, s32 height, const std::string title, const std::string path, s16 libraryId) {
	
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

	this->libDestructor = nullptr;
	if (libID == SDL2_IDX) {
		this->libDestructor = (libDestructorFunc)dlsym(dlPtr, "SDL2LibDestructor");
	} 

	if (!this->winCreate || !this->winClear || !this->winDisplay 
		|| !this->winIsOpen || !this->winClose || !this->winPollEvent) {
		// std::cerr << "Error: dlSym " << dlerror() << std::endl;
		throw std::invalid_argument("Error: Symbole in lib " + path + " not found");
	}

}

/* Initialize the graphics library and create window */
bool GraphicLib::windowCreate() {
    this->window = this->winCreate(width, height, title.c_str());
    return (this->window != nullptr);
}

/* Clear the screen */
void GraphicLib::clear() {
    this->winClear(this->window);
}


/* Display the rendered content */
void GraphicLib::display() {
    this->winDisplay(this->window);
}

/**
 * @brief Color a tile at x, y with r, g, b, a
 * @param x x position of the tile
 * @param y y position of the tile
 * @param rgba color of the tile in rgba
 */
void GraphicLib::colorTile(u32 x, u32 y, u8 r, u8 g, u8 b, u8 a) {
	this->winColorTile(this->window, x, y, r, g, b, a);
}

/* Check if the window is open */
bool GraphicLib::isOpen() {
    return (this->window  && this->winIsOpen(this->window));
}

/* Process events */
void GraphicLib::processEvents(Nibbler &ctx) {
	s32 key = NKEY_INVALID;
	
	/* While until no more valid event */
	while ((key = this->winPollEvent(this->window)) != NKEY_INVALID) {
		if (key == NKEY_ESC) {
			ctx.setIsRunning(0);
			break ;
		} 
		else if ((key == NKEY_1 || key == NKEY_2 || key == NKEY_3) && (key != this->libID)) {
			std::cout << PURPLE << "Switching to lib " << key << RESET << std::endl;
			ctx.setCurrentLibIdx((s32)key);
			this->close();
			break ;
		} 
		else if (key == NKEY_UP || key == NKEY_DOWN || key == NKEY_LEFT || key == NKEY_RIGHT) {
			ctx.getSnake().handleSnakeDir(key);
			break ;
		}
	}
}

/* Close the graphics library and set window to null */
void GraphicLib::close() {
	if (this->window) {
		this->winClose(this->window);
		this->window = nullptr;
	}
}