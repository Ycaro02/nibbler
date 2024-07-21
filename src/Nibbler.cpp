#include "../include/Nibbler.hpp"
#include "../include/SFLib.hpp"
#include "../include/SDL2Lib.hpp"

Nibbler::Nibbler() : width(0), height(0), board(nullptr) {
	libs[0] = nullptr;
	libs[1] = nullptr;
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
	return *this;
}

Nibbler::Nibbler(const Nibbler &ref) {
	*this = ref;
}

void Nibbler::NibblerInitLib(std::string title, std::string path, s32 libID, s32 winWidth, s32 winHeight) {
	if (libID == 0) {
		libs[0] = new SFLib(winWidth, winHeight, title, path);
	} else if (libID == 1) {
		libs[1] = new SDL2Lib(winWidth, winHeight, title, path);
	}
}

Nibbler::Nibbler(s32 width, s32 height) : width(width), height(height) {
	board = new u8*[height];
	for (s32 i = 0; i < height; i++) {
		board[i] = new u8[width];
		for (s32 j = 0; j < width; j++) {
			board[i][j] = 0;
		}
	}
	isRunning = 1;
	currentLib = SFML_IDX;

	/* Load the libraries */

	s32 winWidth = width * TILE_SIZE + width * TILE_SPACING;
	s32 winHeight = height * TILE_SIZE + height * TILE_SPACING;

	winWidth += TILE_SPACING;
	winHeight += TILE_SPACING;

	NibblerInitLib("SFML", "rsc/wrapperlib/SFMLWrapper.so", 0, winWidth, winHeight);
	NibblerInitLib("SDL2", "rsc/wrapperlib/SDL2Wrapper.so", 1, winWidth, winHeight);
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

u8 &Nibbler::boarTileGet(s32 x, s32 y) {
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

AGraphicLib *Nibbler::getCurrentLib() {
	return (libs[currentLib]);
}