#include "../include/Nibbler.hpp"
#include "../include/Snake.hpp"

#include <stdexcept> // for std::invalid_argument

Nibbler::Nibbler() : width(0), height(0), board(nullptr), nbFood(0), currentLib(0), isRunning(0) {
	libs[0] = nullptr;
	libs[1] = nullptr;
	libs[2] = nullptr;
}

static void freeBoard(u8 **board, s32 height) {
	for (s32 i = 0; i < height; i++) {
		delete[] board[i];
	}
	delete[] board;
}


Nibbler::~Nibbler() {
	if (board) {
		freeBoard(board, height);
	}
	delete libs[0];
	delete libs[1];
	delete libs[2];
}

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

Nibbler::Nibbler(const Nibbler &ref) {
	*this = ref;
}

void Nibbler::NibblerInitLib(std::string title, std::string path, s32 libID, s32 winWidth, s32 winHeight) {
	libs[libID] = new GraphicLib(winWidth, winHeight, title, path, libID);
}

void Nibbler::foodAdd() {
	s32 foodY = -1, foodX = -1;
	while (foodY == -1 || foodX == -1) {
		foodY = rand() % getHeight();
		foodX = rand() % getWidth();
		if (boardTileGet(foodX, foodY) != EMPTY) {
			std::cout << "Food already present at " << foodX << " " << foodY << std::endl;
			foodY = -1;
			foodX = -1;
		}
	}
	boardTileSet(foodX, foodY, FOOD);
	setNbFood(getNbFood() + 1);
}

void Nibbler::resetGame() {
	/* Reset the board */
	for (s32 i = 0; i < height; i++) {
		for (s32 j = 0; j < width; j++) {
			board[i][j] = EMPTY;
		}
	}
	/* Initialize the snake at the center of the board */
	snake = Snake(*this, width >> 1, height >> 1);

	/* Initialize the food */
	setNbFood(0);
	foodAdd();
	foodAdd();
}


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


Nibbler::Nibbler(std::string w, std::string h) {
	
	width = parseIntegerData(w);
	height = parseIntegerData(h);

	std::cout << "Width: " << width << " Height: " << height << std::endl;

	/* Initialize basic value */	
	setIsRunning(1);
	setCurrentLibIdx(SFML_IDX);

	/* Alloc the board */
	board = new u8*[height];
	for (s32 i = 0; i < height; i++) {
		board[i] = new u8[width];
	}

	// s32 winWidth = width * TILE_SIZE + width * TILE_SPACING;
	// s32 winHeight = height * TILE_SIZE + height * TILE_SPACING;

	// winWidth += TILE_SPACING;
	// winHeight += TILE_SPACING;

	/* Load the libraries */
	NibblerInitLib("SFML", "rsc/wrapperlib/SFMLWrapper.so", SFML_IDX, WIN_W(width), WIN_H(height));
	NibblerInitLib("SDL2", "rsc/wrapperlib/SDL2Wrapper.so", SDL2_IDX, WIN_W(width), WIN_H(height));
	NibblerInitLib("Raylib", "rsc/wrapperlib/RaylibWrapper.so", RAYLIB_IDX, WIN_W(width), WIN_H(height));

	/* Initialize the random seed for food spawn */
	srand(time(NULL));

	resetGame();
}


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

GraphicLib *Nibbler::getCurrentLib() {
	return (libs[currentLib]);
}

Snake &Nibbler::getSnake() {
	return (snake);
}