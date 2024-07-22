#ifndef NIBBLER_HPP
#define NIBBLER_HPP

#include "AGraphicLib.hpp"
#include "Snake.hpp"

#define TILE_SIZE 64
#define TILE_SPACING 2

class Nibbler {
	public:
	/* Canonical form */
	Nibbler();
	~Nibbler();
	Nibbler& operator=(const Nibbler &ref);
	Nibbler(const Nibbler &ref);


	/* Constructor */
	Nibbler(s32 width, s32 height);
	void NibblerInitLib(std::string title, std::string path, s32 libID, s32 winWidth, s32 winHeight);
	void DisplayBoardFD0();

	void foodAdd();
	void resetGame();

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

	AGraphicLib *getCurrentLib();

	Snake &getSnake();

	private:
	AGraphicLib			*libs[2];
	s32					width;
	s32					height;
	u8					**board;
	s32					nbFood;
	s32					currentLib;
	s32					isRunning;	
	Snake				snake;
};

#endif /* NIBBLER_HPP */