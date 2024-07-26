#ifndef NIBBLER_HPP
#define NIBBLER_HPP

#include "GraphicLib.hpp"
#include "Snake.hpp"
#include <chrono>

typedef std::chrono::steady_clock::time_point ChronoTimePoint;
typedef std::chrono::seconds ChronoSecond;
typedef std::chrono::milliseconds ChronoMilli;


/* Snake move MS */
#define SNAKE_MOVE_MS 300

/* Macro to set bit_nb in buff to val */
#define U32_SET_BIT(buff, bit_nb, val) \
    ((buff) = ((buff) & ~(1 << (bit_nb))) | ((val) << (bit_nb)))

/* Macro to get bit_nb in buff */
#define U32_GET_BIT(buff, bit_nb) (((buff) >> (bit_nb)) & 1)


/* Easy access to the bit */
#define GET_SFML_BIT(buff) (U32_GET_BIT(buff, SFML_IDX))
#define GET_SDL2_BIT(buff) (U32_GET_BIT(buff, SDL2_IDX))
#define GET_RAYLIB_BIT(buff) (U32_GET_BIT(buff, RAYLIB_IDX))
#define GET_RUNNING_BIT(buff) (U32_GET_BIT(buff, RUNNING_IDX))
#define GET_COLOR_BIT(buff) (U32_GET_BIT(buff, COLOR_IDX))

/* Special wrapper for lib idx bit */
#define GET_LIB_IDX(buff) (GET_SFML_BIT(buff) ? SFML_IDX : GET_SDL2_BIT(buff) ? SDL2_IDX : RAYLIB_IDX)
#define RESET_LIB_IDX(buff) (U32_SET_BIT(buff, SFML_IDX, 0), U32_SET_BIT(buff, SDL2_IDX, 0), U32_SET_BIT(buff, RAYLIB_IDX, 0))

/* Special wrapper for running and color bit */
#define SET_RUNNING_BIT(buff, val) (U32_SET_BIT(buff, RUNNING_IDX, val))
#define SET_COLOR_BIT(buff, val) (U32_SET_BIT(buff, COLOR_IDX, val))

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
	void	NibblerInitLib(std::string title, std::string path, std::string textureExt, s32 libID, s32 winWidth, s32 winHeight);

	/* Food handling */
	void	spawnMoreFood();
	void	foodAdd();

	void	resetGame();
	void	checkBoardFull();

	/* Snake handling */
	void	snakeAutoMove();


	/*Getter setter*/
	u8		&boardTileGet(s32 x, s32 y);
	void	boardTileSet(s32 x, s32 y, u8 value);


	s32		&getWidth();
	s32		&getHeight();

	s32		&getNbFood();
	void	setNbFood(s32 value);

	s32		&getEmptyTileNb();
	void	setEmptyTileNb(s32 value);


	u32		getIsRunning();
	void	setIsRunning(u32 value);

	u32		getCurrentLibIdx();
	void	setCurrentLibIdx(u32 libSwitch);

	u32		getColorMode();
	void	setColorMode(u32 value);

	GraphicLib	*getCurrentLib();
	Snake		&getSnake();

	private:
	GraphicLib			*libs[3];		/* Array of the 3 libraries */
	s32					width;			/* Width of the board */
	s32					height;			/* Height of the board */
	u8					**board;		/* Board of the game */
	ChronoTimePoint		lastMove;		/* Last move time */
	ChronoTimePoint		lastFoodSpawn;	/* Last auto move time */
	Snake				snake;			/* Snake object */
	s32					nbFood;			/* Number of food */
	s32					emptyTileNb;	/* Number of empty tile */
	/**
	 * gameState is a u32 that contains the following boolean information:
	 * - Bit 0-2: SFML, SDL2, RAYLIB
	 * - Bit 3: Running
	 * - Bit 4: Color mode
	 * - Bit 5-31: Unused
	 * @note: Use the getter and setter to access the bit
	*/
	u32					gameState;		/* Game state */
};

#endif /* NIBBLER_HPP */