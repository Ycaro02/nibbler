#include "../include/Nibbler.hpp"
#include "../include/Snake.hpp"
#include "../include/Color.hpp"

/* Needed for std::invalid_args */
#include <stdexcept>

/**
 * @brief Free the board
 * @param board Board to free
 * @param height Height of the board
*/
static void freeBoard(u8 **board, s32 height) {
	for (s32 i = 0; i < height; i++) {
		delete[] board[i];
	}
	delete[] board;
}

/**
 * @brief Create a new board
 * @param width Width of the board
 * @param height Height of the board
 * @param value Value to fill the board with
 * @param boardRef Reference to a board to copy
 * @return u8** The new board
 * @note If boardRef is not null, the new board will be a copy of it, otherwise it will be filled with value
 */
static u8** boardNew(s32 width, s32 height, u8 value, u8 **boardRef) {
	u8 **board = new u8*[height];
	for (s32 i = 0; i < height; i++) {
		board[i] = new u8[width];
		for (s32 j = 0; j < width; j++) {
			if (boardRef) {
				board[i][j] = boardRef[i][j];
			} else {
				board[i][j] = value;
			}
		}
	}
	return (board);
}

/* Parse the integer input data */
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

/* Default constructor */
Nibbler::Nibbler()
: width(0), height(0), board(nullptr), lastMove(std::chrono::steady_clock::now())
, lastFoodSpawn(std::chrono::steady_clock::now()), snake(Snake())
, nbFood(0), emptyTileNb(0), gameState(0) {
	libs[0] = nullptr;
	libs[1] = nullptr;
	libs[2] = nullptr;
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
		board = boardNew(width, height, EMPTY, ref.board);
		lastMove = ref.lastMove;
		lastFoodSpawn = ref.lastFoodSpawn;
		snake = ref.snake;
		nbFood = ref.nbFood;
		emptyTileNb = ref.emptyTileNb;
		gameState = ref.gameState;
	}
	return (*this);
}

/* Copy constructor */
Nibbler::Nibbler(const Nibbler &ref) {
	*this = ref;
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

	/* Initialize gameState value */	
	setIsRunning(1);
	setCurrentLibIdx(SFML_IDX);
	setColorMode(0);

	/* Alloc the board */
	board = boardNew(width, height, EMPTY, nullptr);

	/* Load the libraries */
	NibblerInitLib("SFML", "rsc/wrapperlib/SFMLWrapper.so", "png", SFML_IDX, WIN_W(width), WIN_H(height));
	NibblerInitLib("SDL2", "rsc/wrapperlib/SDL2Wrapper.so", "bmp", SDL2_IDX, WIN_W(width), WIN_H(height));
	NibblerInitLib("Raylib", "rsc/wrapperlib/RaylibWrapper.so", "png", RAYLIB_IDX, WIN_W(width), WIN_H(height));

	/* Initialize the random seed for food spawn */
	srand(time(NULL));

	/* This initialise lastMove, lastfoodSpawn, snake, nbFood and emptyTileNB */
	resetGame();
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

	setEmptyTileNb((width * height) - 4);

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
		/* Update direction */
		snake.setDirection(snake.getNextDirection());
		/* Move the snake */
		snake.SnakeMove(*this, snake.getDirection());
		lastMove = now;
	}
}

void Nibbler::spawnMoreFood() {
	ChronoTimePoint now = std::chrono::steady_clock::now();
	ChronoMilli diff = std::chrono::duration_cast<ChronoMilli>(now - lastFoodSpawn);
	if (diff.count() >= 5000 && getNbFood() < 3 && getEmptyTileNb() > 2) {
		foodAdd();
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

Snake &Nibbler::getSnake() {
	return (snake);
}

/* Getters and setters for the game state using get/set bits macro */

u32 Nibbler::getIsRunning() {
	return (GET_RUNNING_BIT(gameState));
}

void Nibbler::setIsRunning(u32 value) {
	SET_RUNNING_BIT(gameState, value);
}

u32 Nibbler::getCurrentLibIdx() {
	return (GET_LIB_IDX(gameState));
}

void Nibbler::setCurrentLibIdx(u32 libSwitch) {
	RESET_LIB_IDX(gameState);
	U32_SET_BIT(gameState, libSwitch, 1);
}

GraphicLib *Nibbler::getCurrentLib() {
	return (libs[GET_LIB_IDX(gameState)]);
}

u32 Nibbler::getColorMode() {
	return (GET_COLOR_BIT(gameState));
}

void Nibbler::setColorMode(u32 value) {
	SET_COLOR_BIT(gameState, value);
}

u32 Nibbler::getPause() {
	return (GET_PAUSE_BIT(gameState));
}

void Nibbler::setPause(u32 value) {
	SET_PAUSE_BIT(gameState, value);
}
