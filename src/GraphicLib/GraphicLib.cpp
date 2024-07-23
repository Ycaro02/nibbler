#include "../../include/GraphicLib.hpp"
#include "../../include/Nibbler.hpp"
#include "../../include/Snake.hpp"
#include "../../include/Color.hpp"

/* Default constructor */
GraphicLib::GraphicLib() 
: dlPtr(nullptr), window(nullptr), winWidth(0), winHeight(0), winTitle("")
, winCreateF(nullptr), winClearF(nullptr), winDisplayF(nullptr), winCloseF(nullptr)
, winIsOpenF(nullptr), winPollEventF(nullptr), libDestructorF(nullptr) {}

/* Assignment operator */
GraphicLib& GraphicLib::operator=(const GraphicLib& ref) {
	if (this != &ref) {
		dlPtr = ref.dlPtr;
		window = ref.window;
		winWidth = ref.winWidth;
		winHeight = ref.winHeight;
		winTitle = ref.winTitle;
		winCreateF = ref.winCreateF;
		winClearF = ref.winClearF;
		winDisplayF = ref.winDisplayF;
		winIsOpenF = ref.winIsOpenF;
		winCloseF = ref.winCloseF;
		winPollEventF = ref.winPollEventF;
		libDestructorF = ref.libDestructorF;
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

	std::cout << YELLOW << "GraphicLib Destructor for " + name + " with :";
	if (window) {
		std::cout << RED << " window close()";
		close();
	}
	if (libDestructorF) {
		std::cout << ORANGE << " libDestructor()";
		libDestructorF();
	}
	if (dlPtr) {
		std::cout << GREEN << " dlclose()" << RESET << std::endl;
		dlclose(dlPtr);
	}
}


static void *loadFuncPtr(void *dlPtr, const std::string &name) {
	void *funcPtr = dlsym(dlPtr, name.c_str());
	if (!funcPtr) {
		throw std::invalid_argument("Error: Symbole " + name + " not found");
	}
	return funcPtr;
}

/**	@brief Graphic lib Constructor 
 *	@param width width of the window
 *	@param height height of the window
 *	@param title title of the window
 *	@param path path to the dynamic library
 *	@param libraryId id of the library (SDL2, SFML, Raylib)
*/
GraphicLib::GraphicLib(s32 width, s32 height, const std::string title, const std::string path, s16 libraryId) {
	
	libID = libraryId;
	winWidth = width;
	winHeight = height;
	winTitle = title;
	window = nullptr;
	dlPtr = dlopen(path.c_str(), RTLD_LAZY);
	if (!dlPtr) {
		throw std::invalid_argument("Error: Graphic lib " + path + " not found");
	}
	winCreateF		= (createWindowFunc)loadFuncPtr(dlPtr, "createWindowWrapper");
    winClearF		= (voidWinFunc)loadFuncPtr(dlPtr, "windowClearWrapper");
    winDisplayF		= (voidWinFunc)loadFuncPtr(dlPtr, "windowDisplayWrapper");
    winCloseF		= (voidWinFunc)loadFuncPtr(dlPtr, "windowCloseWrapper");
    winIsOpenF		= (boolWinFunc)loadFuncPtr(dlPtr, "windowIsOpenWrapper");
    winPollEventF	= (winFuncPollFunc)loadFuncPtr(dlPtr, "windowPollEventWrapper");
	winColorTileF	= (tileColorFunc)loadFuncPtr(dlPtr, "colorTileWrapper");

	libDestructorF = nullptr;
	if (libID == SDL2_IDX) {
		libDestructorF = (libDestructorFunc)dlsym(dlPtr, "SDL2LibDestructor");
	} 
}

/* Initialize the graphics library and create window */
bool GraphicLib::windowCreate() {
    window = winCreateF(winWidth, winHeight, winTitle.c_str());
    return (window != nullptr);
}

/* Clear the screen */
void GraphicLib::clear() {
    winClearF(window);
}


/* Display the rendered content */
void GraphicLib::display() {
    winDisplayF(window);
}

/**
 * @brief Color a tile at x, y with r, g, b, a
 * @param x x position of the tile
 * @param y y position of the tile
 * @param rgba color of the tile in rgba
 */
void GraphicLib::colorTile(u32 x, u32 y, u8 r, u8 g, u8 b, u8 a) {
	winColorTileF(window, x, y, r, g, b, a);
}

/* Check if the window is open */
bool GraphicLib::isOpen() {
    return (window  && winIsOpenF(window));
}

/* Move the snake step by step, debug function for test */
// void moveStepByStep(Nibbler &ctx, s32 event) {

// 	Snake &snake = ctx.getSnake();

// 	if (event == NKEY_UP) {
// 		snake.SnakeMove(ctx, UP);
// 	} else if (event == NKEY_DOWN) {
// 		snake.SnakeMove(ctx, DOWN);
// 	} else if (event == NKEY_LEFT) {
// 		snake.SnakeMove(ctx, LEFT);
// 	} else if (event == NKEY_RIGHT) {
// 		snake.SnakeMove(ctx, RIGHT);
// 	}
// }

/* Process events */
void GraphicLib::processEvents(Nibbler &ctx) {
	s32 key = NKEY_INVALID;
	
	/* While until no more valid event */
	while ((key = winPollEventF(window)) != NKEY_INVALID) {
		if (key == NKEY_ESC) {
			ctx.setIsRunning(0);
			break ;
		} 
		else if ((key == NKEY_1 || key == NKEY_2 || key == NKEY_3) && (key != libID)) {
			std::cout << PURPLE << "Switching to lib " << key << RESET << std::endl;
			ctx.setCurrentLibIdx((s32)key);
			close();
			break ;
		} 
		else if (key == NKEY_UP || key == NKEY_DOWN || key == NKEY_LEFT || key == NKEY_RIGHT) {
			ctx.getSnake().handleSnakeDir(key);
			// moveStepByStep(ctx, key);
			break ;
		}
	}
}

/* Close the graphics library and set window to null */
void GraphicLib::close() {
	if (window) {
		winCloseF(window);
		window = nullptr;
	}
}