#ifndef GraphicLib_HPP
#define GraphicLib_HPP

#include "short_type.hpp"
#include "dlfcn.h"
#include <iostream>
#include <string>

/* Define for the library ID */
#define SFML_IDX 0
#define SDL2_IDX 1
#define RAYLIB_IDX 2

/**	
 * 	Function pointer types for the dynamic library
 *	void* is always the window pointer
*/
typedef void* (*createWindowFunc)(u32, u32, const char*);
typedef void (*voidWinFunc)(void*);
typedef void (*libDestructorFunc)();
typedef void (*tileColorFunc)(void*, u32, u32, u8, u8, u8, u8);
typedef bool (*boolWinFunc)(void*);

/* Function pointer for polling events return normalised key */
typedef s32 (*winFuncPollFunc)(void*);

/* Enum for the keys normalise between lib */
typedef enum NormaliseKey {
	NKEY_INVALID=-1,	/* Invalid key */
	NKEY_1,				/* Key 1 switch to first LIB: SFML */
	NKEY_2,				/* Key 2 switch to second LIB: SDL2 */
	NKEY_3,				/* Key 3 switch to third LIB: RAYLIB */
	NKEY_UP,			/* Key UP, move snake dir to UP */
	NKEY_DOWN,			/* Key DOWN, move snake dir to DOWN */
	NKEY_LEFT,			/* Key LEFT, move snake dir to LEFT */
	NKEY_RIGHT,			/* Key RIGHT, move snake dir to RIGHT */
	NKEY_ESC,			/* Key ESC, close the window */
} NormaliseKey;

/* Forward declaration of Nibbler */
class Nibbler;

/* GraphicLib class to handle multiple lib */
class GraphicLib { 
	public:

	/*Canonical form*/
	GraphicLib();
	~GraphicLib();
	GraphicLib& operator=(const GraphicLib& ref);
	GraphicLib(const GraphicLib& ref);

	/* Real Constructor */
	GraphicLib(s32 width, s32 height, const std::string title, const std::string path, s16 libID);

	/* Initialize the graphics library */
	bool windowCreate();
	/* Clear the screen */
	void clear();
	/* Display the rendered content */
	void display();
	/* Check if the window is open */
	bool isOpen();
	/* Color a tile at x, y with r, g, b, a */
	void colorTile(u32 x, u32 y, u8 r, u8 g, u8 b, u8 a);
	/* Close the graphics library */
	void close();
	/* Process events */
	void processEvents(Nibbler &ctx);

	private:

	void 				*dlPtr;			/* Pointer to the dynamic library */
    void				*window;		/* Pointer to the window */
    s32					winWidth;		/* Width of the window */
    s32					winHeight;		/* Height of the window */
    std::string			winTitle;		/* Title of the window */
	s16					libID;			/* ID of the library */

    createWindowFunc	winCreateF;		/* Function pointer to createWindow */
    voidWinFunc			winClearF;		/* Function pointer to windowClear */
    voidWinFunc			winDisplayF;	/* Function pointer to windowDisplay */
    voidWinFunc			winCloseF;		/* Function pointer to windowClose */
    boolWinFunc			winIsOpenF;		/* Function pointer to windowIsOpen */
    winFuncPollFunc		winPollEventF;	/* Function pointer to windowPollEvent */
	libDestructorFunc	libDestructorF;	/* Function pointer to libDestructor */
	tileColorFunc		winColorTileF;	/* Function pointer to colorTile */
};



#endif /* GraphicLib_HPP */