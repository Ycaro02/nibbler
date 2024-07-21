#ifndef NIBBLER_HPP
#define NIBBLER_HPP

#include "AGraphicLib.hpp"


typedef enum BoardState_e {
	EMPTY = 0U,
	FOOD = 1U,
	SNAKE_HEAD = 2U
} BoardState;

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

	u8 &boarTileGet(s32 x, s32 y);
	void boardTileSet(s32 x, s32 y, u8 value);

	s32 &getWidth();
	s32 &getHeight();

	s32 &getIsRunning();
	void setIsRunning(s32 value);

	s32 &getCurrentLibIdx();
	void setCurrentLibIdx(s32 value);

	AGraphicLib *getCurrentLib();

	private:
	AGraphicLib			*libs[2];
	s32					width;
	s32					height;
	u8					**board;
	s32					currentLib;
	s32					isRunning;	
};

#endif /* NIBBLER_HPP */