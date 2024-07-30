#ifndef GraphicLib_HPP
#define GraphicLib_HPP

#include "BasicDefine.hpp"
#include "dlfcn.h"
#include <iostream>
#include <string>


/*	
	* Define the texture path use as follow:
	* TEXTURE_DIR + libName + TEXTURE_NAME + TEXTURE_EXT
 */
#define TEXTURE_DIR "rsc/texture/"

/* Define the TEXTURE_NAME without extension */
#define HEAD_L_FILE "/HeadLeft."
#define HEAD_R_FILE "/HeadRight."
#define HEAD_BOT_FILE "/HeadBot."
#define HEAD_TOP_FILE "/HeadTop."
#define BODY_FILE "/Body."
#define FOOD_FILE "/Food."
#define EMPTY_FILE "/Empty."

// #define HEAD_TEST_FILE "/HeadTest."
// #define BODY_TEST_FILE "/BodyTest." 

/* Define height for top band info */
#define TOP_BAND_HEIGHT 100

/* Define font info : path, size, mult per char for align/center text */
#define FONT_PATH "rsc/font/arial.ttf"
#define FONT_SIZE 40
#define FONT_MULT 12

typedef enum textureIdx {
	HEAD_LEFT_IDX = 0,
	HEAD_RIGHT_IDX,
	HEAD_BOT_IDX,
	HEAD_TOP_IDX,
	BODY_IDX,
	FOOD_IDX,
	EMPTY_IDX,
	TEXTURE_MAX
} textureIdx;

/* Define for the library ID and game state */
#define SFML_IDX 0
#define SDL2_IDX 1
#define RAYLIB_IDX 2
#define RUNNING_IDX 3
#define COLOR_IDX 4
#define PAUSE_IDX 5

/**	
 * 	Function pointer types for the dynamic library
 *	The first void* args is always the window pointer except for unloadTextFunc, unloadFontFunc
*/

/* Here u32 are width and height for the window, const char* the window title */
typedef void *(*createWindowFunc)(u32, u32, const char*);
typedef void (*voidWinFunc)(void*);
typedef void (*libDestructorFunc)();
typedef bool (*boolWinFunc)(void*);

/* Function pointer for polling events return normalised key */
typedef s32 (*winFuncPollFunc)(void*);

/* Here first iVec is tilePos, second is scale, last is u32 RGBA color */
typedef void (*tileColorFunc)(void*, iVec2, iVec2, u32);

/* Here const char* is the path to the texture, return the allocated texture loaded */
typedef void *(*loadTextFunc)(void *, const char*);

/* Here void* is the texture to unload */
typedef void (*unloadTextFunc)(void *);

/* Here first void* are window, second texture, iVec2 are x,y coordonate and second texture scaling */
typedef void (*drawTextFunc)(void *, void *, iVec2, iVec2);

/* Here const char* is the path to the font, return the allocated font loaded */
typedef void *(*loadFontFunc)(const char*);

/* Here void* is the font to unload */
typedef void (*unloadFontFunc)(void*);

/* Here first void* font, 2nd void* texture, char* text to write, iVec2 is the position, u32 font_size, u32 color */
typedef void (*writeTextFunc)(void*, void*, const char*, iVec2, u32, u32);

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
	NKEY_A,				/* Key A, switch color mode */
	NKEY_P,				/* Key P, pause the game */
	NKEY_ESC,			/* Key ESC, close the window */
} NormaliseKey;

/* Macro to Compute the window Height, width */
#define WIN_W(w) (s32)((w * TILE_SIZE) + (w * TILE_SPACING) + TILE_SPACING)
#define WIN_H(h) (s32)((h * TILE_SIZE) + (h * TILE_SPACING) + TILE_SPACING + TOP_BAND_HEIGHT)


/* Macro to convert RGBA to UINT32 and UINT32 to RGBA */
#define		RGBA_TO_UINT32(r, g, b, a)		((u32)((a << 24) | (r << 16) | (g << 8) | b))

#define		UINT32_TO_RGBA(color, r, g, b, a)	\
	do { \
		r = (color >> 16) & 0xFF;				\
		g = (color >> 8) & 0xFF;				\
		b = color & 0xFF;						\
		a = (color >> 24) & 0xFF;				\
	} while (0)

/* Macro to convert tile position to pixel */
#define TILE_POSITION_TO_PIXEL(p, px, py) \
    do { \
        (px) = (p).x * TILE_SIZE + ((p).x + 1) * TILE_SPACING; \
        (py) = ((p).y * TILE_SIZE + ((p).y + 1) * TILE_SPACING) + TOP_BAND_HEIGHT; \
    } while (0)


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
	GraphicLib(s32 width, s32 height, const std::string title, const std::string path, const std::string ext, s16 libID);

	/* Initialize the graphics library */
	bool windowCreate();
	/* Clear the screen */
	void clear();
	/* Display the rendered content */
	void display();
	/* Check if the window is open */
	bool isOpen();
	/* Color a tile at x, y with r, g, b, a */
	void colorTile(iVec2 tilePos, iVec2 scale, u32 color);

	/* Load a texture */
	void *loadTexture(const char *path);
	/* Unload a texture */
	void unloadTexture(void *texture);

	/* Load a font */
	void *loadFont(const char *path);
	/* Unload a font */
	void unloadFont(void *font);

	/* Write a text */
	void writeText(const char *text, iVec2 pos, u32 fontSize, u32 color);

	/* Draw a texture */
	// void drawTextureTile(void *texture, u32 x, u32 y);
	void drawTextureTile(void *texture, iVec2 tilePos, iVec2 scale);

	/* Close the graphics library */
	void close();

	/* Process events */
	void processEvents(Nibbler &ctx);

	/* Getter for the window width height */
	s32 getWidth() const;
	s32 getHeight() const;
	std::string getTitle() const;
	std::string getTextName(std::string name) const;

	void *getTexture(s32 id) const;

	private:

	/* Classic attributs */
	void 				*dlPtr;			/* Pointer to the dynamic library */
    void				*window;		/* Pointer to the window */
	void				*texture[TEXTURE_MAX];	/* Pointer to the texture */
	std::string			textureExt;	/* Path to the texture */
    s32					winWidth;		/* Width of the window */
    s32					winHeight;		/* Height of the window */
    std::string			winTitle;		/* Title of the window */
	s16					libID;			/* ID of the library */
	void				*font;			/* Pointer to the font */

	/* Function pointers */

    createWindowFunc	winCreateF;		/* Function pointer to createWindow */
    voidWinFunc			winClearF;		/* Function pointer to windowClear */
    voidWinFunc			winDisplayF;	/* Function pointer to windowDisplay */
    voidWinFunc			winCloseF;		/* Function pointer to windowClose */
    boolWinFunc			winIsOpenF;		/* Function pointer to windowIsOpen */
    winFuncPollFunc		winPollEventF;	/* Function pointer to windowPollEvent */
	libDestructorFunc	libDestructorF;	/* Function pointer to libDestructor */
	tileColorFunc		winColorTileF;	/* Function pointer to colorTile */
	loadTextFunc		loadTextF;		/* Function pointer to loadTexture */
	unloadTextFunc		unloadTextF;	/* Function pointer to unloadTexture */
	drawTextFunc		drawTextF;		/* Function pointer to drawTexture tile */
	loadFontFunc		loadFontF;		/* Function pointer to loadFont */
	unloadFontFunc		unloadFontF;	/* Function pointer to unloadFont */
	writeTextFunc		writeTextF;		/* Function pointer to writeText */
};



#endif /* GraphicLib_HPP */