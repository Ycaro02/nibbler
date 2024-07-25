#include "../../include/GraphicLib.hpp"
#include "../../include/Nibbler.hpp"
#include "../../include/Snake.hpp"
#include "../../include/Color.hpp"

/* Default constructor */
GraphicLib::GraphicLib() 
: dlPtr(nullptr), window(nullptr), winWidth(0), winHeight(0), winTitle(""), libID(0)
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
	std::cout << ORANGE << " libDestructor()" << GREEN << " dlclose()" << RESET << std::endl;
	libDestructorF();
	if (dlPtr) {
		dlclose(dlPtr);
	}
}


static void *loadFuncPtr(void *dlPtr, const std::string &name, const std::string &libName) {
	void *funcPtr = dlsym(dlPtr, name.c_str());
	if (!funcPtr) {
		throw std::invalid_argument("Error: Symbole " + name + " not found in : " + libName);
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
GraphicLib::GraphicLib(s32 width, s32 height, const std::string title, const std::string path, const std::string textExt, s16 libraryId) {
	
	libID = libraryId;
	winWidth = width;
	winHeight = height;
	winTitle = title;
	window = nullptr;
	
	for (u32 i = 0; i < 4; i++) {
		texture[i] = nullptr;
	}
	texturePath = textExt;
	dlPtr = dlopen(path.c_str(), RTLD_LAZY);
	if (!dlPtr) {
		std::cerr << "Error: " << dlerror() << std::endl;
		throw std::invalid_argument("Error: Graphic lib |" + path + "| not found");
	}
	winCreateF		= (createWindowFunc)loadFuncPtr(dlPtr, "createWindowWrapper", path);
    winClearF		= (voidWinFunc)loadFuncPtr(dlPtr, "windowClearWrapper", path);
    winDisplayF		= (voidWinFunc)loadFuncPtr(dlPtr, "windowDisplayWrapper", path);
    winCloseF		= (voidWinFunc)loadFuncPtr(dlPtr, "windowCloseWrapper", path);
    winIsOpenF		= (boolWinFunc)loadFuncPtr(dlPtr, "windowIsOpenWrapper", path);
    winPollEventF	= (winFuncPollFunc)loadFuncPtr(dlPtr, "windowPollEventWrapper", path);
	winColorTileF	= (tileColorFunc)loadFuncPtr(dlPtr, "colorTileWrapper", path);
	libDestructorF = (libDestructorFunc)loadFuncPtr(dlPtr, "libDestructorWrapper", path);
	loadTextF		= (loadTextFunc)loadFuncPtr(dlPtr, "loadTextureWrapper", path);
	unloadTextF		= (unloadTextFunc)loadFuncPtr(dlPtr, "unloadTextureWrapper", path);
	drawTextF		= (drawTextFunc)loadFuncPtr(dlPtr, "drawTextureTileWrapper", path);
}

/* Initialize the graphics library and create window */
bool GraphicLib::windowCreate() {
    window = winCreateF(winWidth, winHeight, winTitle.c_str());

	std::string head_path = HEAD_path + texturePath;
	std::string body_path = BODY_path + texturePath;
	std::string empty_path = EMPTY_path + texturePath;
	std::string food_path = FOOD_path + texturePath;

	// std::cout << "Head path: " << head_path << std::endl;
	// std::cout << "Body path: " << body_path << std::endl;
	// std::cout << "Empty path: " << empty_path << std::endl;
	// std::cout << "Food path: " << food_path << std::endl;

	texture[HEAD_IDX] = loadTexture(head_path.c_str());
    texture[BODY_IDX] = loadTexture(body_path.c_str());
	texture[EMPTY_IDX] = loadTexture(empty_path.c_str());
	texture[FOOD_IDX] = loadTexture(food_path.c_str());
	
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
		else if ((key == NKEY_1 || key == NKEY_2 || key == NKEY_3) && (key != ctx.getCurrentLibIdx())) {
			std::cout << PURPLE << "Switching to lib " << key << RESET << std::endl;
			ctx.setCurrentLibIdx((s32)key);
			close();
			break ;
		} 
		else if (key == NKEY_UP || key == NKEY_DOWN || key == NKEY_LEFT || key == NKEY_RIGHT) {
			ctx.getSnake().handleSnakeDir(key);
			// moveStepByStep(ctx, key);
			break ;
		} else {
			break ;
		}
	}
}

void *GraphicLib::loadTexture(const char *path) {
	return loadTextF(window, path);
}

void GraphicLib::unloadTexture(void *texture) {
	unloadTextF(texture);
}

void GraphicLib::drawTextureTile(void *texture, u32 x, u32 y) {
	drawTextF(window, texture, x, y);
}

/* Close the graphics library and set window to null */
void GraphicLib::close() {
	for (u32 i = 0; i < 4; i++) {
		if (texture[i]) {
			unloadTexture(texture[i]);
			texture[i] = nullptr;
		}
	}

	if (window) {
		winCloseF(window);
		window = nullptr;
	}
}

void *GraphicLib::getTexture(s32 id) const {
	if (id > 4) {
		return (nullptr);
	}
	return (texture[id]);
}