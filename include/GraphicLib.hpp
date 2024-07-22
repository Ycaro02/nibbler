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
	NKEY_INVALID=-1,
	NKEY_1,
	NKEY_2,
	NKEY_3,
	NKEY_UP,
	NKEY_DOWN,
	NKEY_LEFT,
	NKEY_RIGHT,
	NKEY_ESC,
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
    s32					width;			/* Width of the window */
    s32					height;			/* Height of the window */
    std::string			title;			/* Title of the window */
	s16					libID;			/* ID of the library */

    createWindowFunc	winCreate;		/* Function pointer to createWindow */
    voidWinFunc			winClear;		/* Function pointer to windowClear */
    voidWinFunc			winDisplay;		/* Function pointer to windowDisplay */
    voidWinFunc			winClose;		/* Function pointer to windowClose */
    boolWinFunc			winIsOpen;		/* Function pointer to windowIsOpen */
    winFuncPollFunc		winPollEvent;	/* Function pointer to windowPollEvent */
	libDestructorFunc	libDestructor;	/* Function pointer to libDestructor */
	tileColorFunc		winColorTile;	/* Function pointer to colorTile */

};



#endif /* GraphicLib_HPP */