#include "../../include/GraphicLib.hpp"
#include "../../include/Nibbler.hpp"
#include "../../include/Snake.hpp"
#include "../../include/Color.hpp"
#include "../../include/Menu.hpp"
#include "../../include/HandleAction.hpp"

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
	
	if (menu) {
		delete menu;
		menu = nullptr;
		std::cout << RED << " menu delete()";
	}
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

static Menu *initMenu(s32 windowWidth, s32 windowHeight) {
	iVec2 startMenu, sizeMenu, textPause;
	std::string pause = "PAUSE";

	sizeMenu.x = windowWidth >> 1;
	sizeMenu.y = (windowHeight - TOP_BAND_HEIGHT) >> 1;
	startMenu.x = sizeMenu.x >> 1;
	startMenu.y = (sizeMenu.y >> 1) + TOP_BAND_HEIGHT;
	textPause.x = startMenu.x + (sizeMenu.x - startMenu.x) - (pause.size() * FONT_MULT);
	textPause.y = startMenu.y;

	return (new Menu(startMenu, sizeMenu, textPause, 4));
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
	textureExt = textExt;
	dlPtr = dlopen(path.c_str(), RTLD_LAZY);
	if (!dlPtr) {
		std::cerr << "Error: " << dlerror() << std::endl;
		throw std::invalid_argument("Error: Graphic lib |" + path + "| not found");
	}
	winCreateF		=	(createWindowFunc)loadFuncPtr(dlPtr, "createWindowWrapper", path);
    winClearF		=	(voidWinFunc)loadFuncPtr(dlPtr, "windowClearWrapper", path);
    winDisplayF		=	(voidWinFunc)loadFuncPtr(dlPtr, "windowDisplayWrapper", path);
    winCloseF		=	(voidWinFunc)loadFuncPtr(dlPtr, "windowCloseWrapper", path);
    winIsOpenF		=	(boolWinFunc)loadFuncPtr(dlPtr, "windowIsOpenWrapper", path);
    winPollEventF	=	(winFuncPollFunc)loadFuncPtr(dlPtr, "windowPollEventWrapper", path);
	winColorTileF	=	(tileColorFunc)loadFuncPtr(dlPtr, "colorTileWrapper", path);
	libDestructorF	=	(libDestructorFunc)loadFuncPtr(dlPtr, "libDestructorWrapper", path);
	loadTextF		=	(loadTextFunc)loadFuncPtr(dlPtr, "loadTextureWrapper", path);
	unloadTextF		=	(unloadTextFunc)loadFuncPtr(dlPtr, "unloadTextureWrapper", path);
	drawTextF		=	(drawTextFunc)loadFuncPtr(dlPtr, "drawTextureTileWrapper", path);
	loadFontF		=	(loadFontFunc)loadFuncPtr(dlPtr, "loadFontWrapper", path);
	unloadFontF		=	(unloadFontFunc)loadFuncPtr(dlPtr, "unloadFontWrapper", path);
	writeTextF		=	(writeTextFunc)loadFuncPtr(dlPtr, "writeTextWrapper", path);

	menu = initMenu(getWidth(), getHeight());

}

std::string GraphicLib::getTextName(std::string name) const {
	return (TEXTURE_DIR + winTitle + name + textureExt);
}

/* Initialize the graphics library and create window */
bool GraphicLib::windowCreate() {
    window = winCreateF(winWidth, winHeight, winTitle.c_str());


	texture[HEAD_LEFT_IDX] = loadTexture(getTextName(HEAD_L_FILE).c_str());
	texture[HEAD_RIGHT_IDX] = loadTexture(getTextName(HEAD_R_FILE).c_str());
	texture[HEAD_BOT_IDX] = loadTexture(getTextName(HEAD_BOT_FILE).c_str());
	texture[HEAD_TOP_IDX] = loadTexture(getTextName(HEAD_TOP_FILE).c_str());
	texture[BODY_IDX] = loadTexture(getTextName(BODY_FILE).c_str());
	texture[FOOD_IDX] = loadTexture(getTextName(FOOD_FILE).c_str());
	texture[EMPTY_IDX] = loadTexture(getTextName(EMPTY_FILE).c_str());

	for (u32 i = 0; i < TEXTURE_MAX; i++) {
		if (!texture[i]) {
			std::cerr << "Error: Texture " << i << " not found" << std::endl;
			return (false);
		}
	}

	std::string btnUnpress = TEXTURE_DIR + winTitle + "/buttonUnpress." + textureExt;
	std::string btnPress = TEXTURE_DIR + winTitle + "/buttonPress." + textureExt;
	menu->setTextureBtnPress(loadTexture(btnPress.c_str()));
	menu->setTextureBtnUnpress(loadTexture(btnUnpress.c_str()));
	if (menu->getTextureBtnPress() == nullptr || menu->getTextureBtnUnpress() == nullptr) {
		std::cerr << "Error: Button texture not found press : " << btnUnpress << " Unpress : " << btnPress <<  std::endl;
		throw std::invalid_argument("Error: Button texture not found");
	}


	font = loadFont(FONT_PATH);

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
// void GraphicLib::colorTile(u32 x, u32 y, u8 r, u8 g, u8 b, u8 a) {
void GraphicLib::colorTile(iVec2 tilePos, iVec2 scale, u32 color) {
	(void)scale;
	// winColorTileF(window, tilePos.x, tilePos.y, color);
	winColorTileF(window, tilePos, scale, color);
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
	u32 key = winPollEventF(window);
	ctx.getActionHandler()->actionProcess(key);
}

void *GraphicLib::loadFont(const char *path) {
	return (loadFontF(path));
}

void GraphicLib::unloadFont(void *font) {
	unloadFontF(font);
}

void GraphicLib::writeText(const char *text, iVec2 pos, u32 fontSize, u32 color) {
	writeTextF(window, font, text, pos, fontSize, color);
}

void *GraphicLib::loadTexture(const char *path) {
	return (loadTextF(window, path));
}

void GraphicLib::unloadTexture(void *texture) {
	unloadTextF(texture);
}

void GraphicLib::drawTextureTile(void *texture, iVec2 tilePos, iVec2 scale) {
	// drawTextF(window, texture, x, y);
	drawTextF(window, texture, tilePos, scale);
}

/* Close the graphics library and set window to null */
void GraphicLib::close() {
	for (u32 i = 0; i < 4; i++) {
		if (texture[i]) {
			unloadTexture(texture[i]);
			texture[i] = nullptr;
		}
	}

	if (menu) {
		if (menu->getTextureBtnPress()) {
			unloadTexture(menu->getTextureBtnPress());
		}
		if (menu->getTextureBtnUnpress()) {
			unloadTexture(menu->getTextureBtnUnpress());
		}
	}

	if (window) {
		winCloseF(window);
		window = nullptr;
	}
	if (font) {
		unloadFont(font);
		font = nullptr;
	}
}

void GraphicLib::drawPauseMenu() {
	menu->displayMenu(this);
}

void *GraphicLib::getTexture(s32 id) const {
	if (id > TEXTURE_MAX) {
		return (nullptr);
	}
	return (texture[id]);
}

s32 GraphicLib::getWidth() const {
	return (winWidth);
}

s32 GraphicLib::getHeight() const {
	return (winHeight);
}

std::string GraphicLib::getTitle() const {
	return (winTitle);
}

Menu *GraphicLib::getMenu() {
	return (menu);
}


/* Old code to handle action */
// if (key == NKEY_ESC) {
// 	ctx.setPause(!(ctx.getPause()));
// 	// ctx.setIsRunning(0);
// } 
// else if (key != ctx.getCurrentLibIdx() && (key == NKEY_1 || key == NKEY_2 || key == NKEY_3)) {
// 	std::cout << PURPLE << "Switching to lib " << key << RESET << std::endl;
// 	ctx.setCurrentLibIdx((s32)key);
// 	close();
// } 
// else if (!ctx.getPause() && (key == NKEY_UP || key == NKEY_DOWN || key == NKEY_LEFT || key == NKEY_RIGHT)) {
// 	ctx.getSnake().handleSnakeDir(key);
// 	// moveStepByStep(ctx, key);
// }
// else if (ctx.getPause() && (key == NKEY_UP || key == NKEY_DOWN || key == NKEY_ENTER)) {
// 	ctx.getCurrentLib()->getMenu()->handleMenu(ctx, key);
// }
// else if (key == NKEY_A) {
// 	ctx.setColorMode(!(ctx.getColorMode()));
// } else if (key == NKEY_P) {
// 	ctx.setPause(!(ctx.getPause()));
// }