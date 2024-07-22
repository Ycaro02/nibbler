#ifndef NIBBLER_HPP
#define NIBBLER_HPP

#include "GraphicLib.hpp"
#include "Snake.hpp"

#define TILE_SIZE 40
#define TILE_SPACING 2


#define WIN_W(w) (s32)((w * TILE_SIZE) + (w * TILE_SPACING) + TILE_SPACING)
#define WIN_H(h) (s32)((h * TILE_SIZE) + (h * TILE_SPACING) + TILE_SPACING)

class Nibbler {
	public:
	/* Canonical form */
	Nibbler();
	~Nibbler();
	Nibbler& operator=(const Nibbler &ref);
	Nibbler(const Nibbler &ref);


	/* Constructor */
	Nibbler(std::string w, std::string h);
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

	GraphicLib *getCurrentLib();

	Snake &getSnake();

	private:
	GraphicLib			*libs[3];
	s32					width;
	s32					height;
	u8					**board;
	s32					nbFood;
	s32					currentLib;
	s32					isRunning;	
	Snake				snake;
};

#endif /* NIBBLER_HPP */