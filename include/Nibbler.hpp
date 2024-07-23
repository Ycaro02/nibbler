#ifndef NIBBLER_HPP
#define NIBBLER_HPP

#include "GraphicLib.hpp"
#include "Snake.hpp"
#include <chrono>

typedef std::chrono::steady_clock::time_point ChronoTimePoint;
typedef std::chrono::seconds ChronoSecond;
typedef std::chrono::milliseconds ChronoMilli;

/* Define tile size for the board */
#define TILE_SIZE 40
#define TILE_SPACING 2

/* Snake move MS */
#define SNAKE_MOVE_MS 500

/* Define the window value */
#define WIN_W(w) (s32)((w * TILE_SIZE) + (w * TILE_SPACING) + TILE_SPACING)
#define WIN_H(h) (s32)((h * TILE_SIZE) + (h * TILE_SPACING) + TILE_SPACING)

/* Nibbler class to handle the game (context)*/
class Nibbler {
	public:
	/* Canonical form */
	Nibbler();
	~Nibbler();
	Nibbler& operator=(const Nibbler &ref);
	Nibbler(const Nibbler &ref);


	/* Constructor */
	Nibbler(std::string w, std::string h);
	void NibblerInitLib(std::string title, std::string path, std::string texturePath, s32 libID, s32 winWidth, s32 winHeight);
	void DisplayBoardFD0();

	/* Food handling */
	void spawnMoreFood();
	void foodAdd();

	void resetGame();
	void checkBoardFull();

	/* Snake handling */
	void snakeAutoMove();


	/*Getter setter*/
	u8 &boardTileGet(s32 x, s32 y);
	void boardTileSet(s32 x, s32 y, u8 value);


	s32 &getWidth();
	s32 &getHeight();

	s32 &getIsRunning();
	void setIsRunning(s32 value);

	s32 &getCurrentLibIdx();
	void setCurrentLibIdx(s32 value);

	s32 &getNbFood();
	void setNbFood(s32 value);

	s32 &getEmptyTileNb();
	void setEmptyTileNb(s32 value);

	GraphicLib *getCurrentLib();
	Snake &getSnake();

	private:
	GraphicLib			*libs[3];		/* Array of the 3 libraries */
	s32					width;			/* Width of the board */
	s32					height;			/* Height of the board */
	u8					**board;		/* Board of the game */
	s32					nbFood;			/* Number of food */
	s32					currentLib;		/* Current library index */
	s32					isRunning;		/* Game state */
	s32					emptyTileNb;	/* Number of empty tile */
	ChronoTimePoint		lastMove;		/* Last move time */
	ChronoTimePoint		lastFoodSpawn;	/* Last auto move time */
	Snake				snake;			/* Snake object */
};

#endif /* NIBBLER_HPP */