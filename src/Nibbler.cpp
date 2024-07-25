#include "../include/Nibbler.hpp"
#include "../include/Snake.hpp"
#include "../include/Color.hpp"

/* Needed for std::invalid_args */
#include <stdexcept>

/* Default constructor */
Nibbler::Nibbler() 
: width(0), height(0), board(nullptr), nbFood(0), currentLib(0), isRunning(0), colorMode(0), emptyTileNb(0),
lastMove(std::chrono::steady_clock::now()), lastFoodSpawn(std::chrono::steady_clock::now()), snake(Snake()) {
	libs[0] = nullptr;
	libs[1] = nullptr;
	libs[2] = nullptr;
}

/* Free the board */
static void freeBoard(u8 **board, s32 height) {
	for (s32 i = 0; i < height; i++) {
		delete[] board[i];
	}
	delete[] board;
}

/* Destructor */
Nibbler::~Nibbler() {
	if (board) {
		freeBoard(board, height);
	}
	delete libs[0];
	delete libs[1];
	delete libs[2];
}

/* Assignment operator */
Nibbler& Nibbler::operator=(const Nibbler &ref) {
	if (this != &ref) {
		width = ref.width;
		height = ref.height;
		if (board) {
			freeBoard(board, height);
		}
		board = new u8*[height];
		for (s32 i = 0; i < height; i++) {
			board[i] = new u8[width];
			for (s32 j = 0; j < width; j++) {
				board[i][j] = ref.board[i][j];
			}
		}
	}
	return (*this);
}

/* Copy constructor */
Nibbler::Nibbler(const Nibbler &ref) {
	*this = ref;
}

/* Initialize lib wrapper */
void Nibbler::NibblerInitLib(std::string title, std::string path, std::string textureExt, s32 libID, s32 winWidth, s32 winHeight) {
	libs[libID] = new GraphicLib(winWidth, winHeight, title, path, textureExt, libID);
}

void Nibbler::checkBoardFull() {
	s32 emptyNb = 0;
	for (s32 i = 0; i < height; i++) {
		for (s32 j = 0; j < width; j++) {
			if (board[i][j] == EMPTY) {
				emptyNb++;
			}
		}
	}
	if (emptyNb == 0) {
		std::cout << GREEN << "Congratulations, you won!" << RESET << std::endl;
		resetGame();
	}
	setEmptyTileNb(emptyNb);
}

/* Add food to the board */
void Nibbler::foodAdd() {
	s32 foodY = -1, foodX = -1;

	if (emptyTileNb == 0) {
		std::cout << ORANGE << "No more space for food empty tile: " << emptyTileNb << RESET << std::endl;
		return ;
	}

	while (foodY == -1 || foodX == -1) {
		foodY = rand() % getHeight();
		foodX = rand() % getWidth();
		if (boardTileGet(foodX, foodY) != EMPTY) {
			// std::cout << "Food already present at " << foodX << " " << foodY << std::endl;
			foodY = -1;
			foodX = -1;
		}
	}
	boardTileSet(foodX, foodY, FOOD);
	setNbFood(getNbFood() + 1);
	lastFoodSpawn = std::chrono::steady_clock::now();
}

/* Reset the game */
void Nibbler::resetGame() {
	lastMove = std::chrono::steady_clock::now();
	/* Reset the board */
	for (s32 i = 0; i < height; i++) {
		for (s32 j = 0; j < width; j++) {
			board[i][j] = EMPTY;
		}
	}
	/* Initialize the snake at the center of the board */
	snake = Snake(*this, width >> 1, height >> 1);

	setEmptyTileNb((width * height) - 5);

	/* Initialize the food */
	setNbFood(0);
	foodAdd();
	foodAdd();
	/* Check if the board is full, this init emptyTileNb */
	checkBoardFull();
}

void Nibbler::snakeAutoMove() {
	ChronoTimePoint now = std::chrono::steady_clock::now();
	ChronoMilli diff = std::chrono::duration_cast<ChronoMilli>(now - lastMove);
	if (diff.count() >= SNAKE_MOVE_MS) {
		snake.SnakeMove(*this, snake.getDirection());
		lastMove = now;
	}
	return ;
}

void Nibbler::spawnMoreFood() {
	ChronoTimePoint now = std::chrono::steady_clock::now();
	ChronoMilli diff = std::chrono::duration_cast<ChronoMilli>(now - lastFoodSpawn);
	if (diff.count() >= 5000 && getNbFood() < 3 && getEmptyTileNb() > 2) {
		foodAdd();
	}
}

/* Parse the integer data */
static int parseIntegerData(const std::string &line) {
    s32 nb = -1;
	
	try {
        nb = std::stoi(line);
    } catch (const std::exception& e) {
		throw std::invalid_argument("Invalid integer data in Nibbler constructor: " + line);
    }
	if (nb <= 4) {
		throw std::invalid_argument("Integer data in Nibbler constructor must be greater than 4");
	} else if (nb >= 30) {
		throw std::invalid_argument("Integer data in Nibbler constructor must be less than 30");
	}

	return (nb);
}

/**	@brief Real Nibbler Constructor
 *	@param w Width of the board
 *	@param h Height of the board
 *	This function can throw an exception if the input data is invalid or if we can't load any library
*/
Nibbler::Nibbler(std::string w, std::string h) {
	
	width = parseIntegerData(w);
	height = parseIntegerData(h);

	std::cout << "Width: " << width << " Height: " << height << std::endl;

	/* Initialize basic value */	
	setIsRunning(1);
	setCurrentLibIdx(SFML_IDX);
	setColorMode(0);

	/* Alloc the board */
	board = new u8*[height];
	for (s32 i = 0; i < height; i++) {
		board[i] = new u8[width];
	}

	/* Load the libraries */
	NibblerInitLib("SFML", "rsc/wrapperlib/SFMLWrapper.so", "png", SFML_IDX, WIN_W(width), WIN_H(height));
	NibblerInitLib("SDL2", "rsc/wrapperlib/SDL2Wrapper.so", "bmp", SDL2_IDX, WIN_W(width), WIN_H(height));
	NibblerInitLib("Raylib", "rsc/wrapperlib/RaylibWrapper.so", "png", RAYLIB_IDX, WIN_W(width), WIN_H(height));
	// NibblerInitLib("SFML", "srsc/wrapperlib/SDL2Wrapper.so", "png", SFML_IDX, WIN_W(width), WIN_H(height));

	/* Initialize the random seed for food spawn */
	srand(time(NULL));

	resetGame();
}

/* Display the board in fd 0 DEBUG FUNC */
void Nibbler::DisplayBoardFD0() {
	for (s32 i = 0; i < height; i++) {
		for (s32 j = 0; j < width; j++) {
			std::cout << (u8)(board[i][j] + '0');
		}
		std::cout << std::endl;
	}
}


/* Getters and setters */

u8 &Nibbler::boardTileGet(s32 x, s32 y) {
	return (board[y][x]);
}

void Nibbler::boardTileSet(s32 x, s32 y, u8 value) {
	board[y][x] = value;
}

s32 &Nibbler::getWidth() {
	return (width);
}

s32 &Nibbler::getHeight() {
	return (height);
}

s32 &Nibbler::getIsRunning() {
	return (isRunning);
}

void Nibbler::setIsRunning(s32 value) {
	isRunning = value;
}

s32 &Nibbler::getCurrentLibIdx() {
	return (currentLib);
}

void Nibbler::setCurrentLibIdx(s32 value) {
	currentLib = value;
}

s32 &Nibbler::getNbFood() {
	return (nbFood);
}

void Nibbler::setNbFood(s32 value) {
	nbFood = value;
}

void Nibbler::setEmptyTileNb(s32 value) {
	emptyTileNb = value;
}

s32 &Nibbler::getEmptyTileNb() {
	return (emptyTileNb);
}


GraphicLib *Nibbler::getCurrentLib() {
	return (libs[currentLib]);
}

s32 &Nibbler::getColorMode() {
	return (colorMode);
}

void Nibbler::setColorMode(s32 value) {
	colorMode = value;
}

Snake &Nibbler::getSnake() {
	return (snake);
}

