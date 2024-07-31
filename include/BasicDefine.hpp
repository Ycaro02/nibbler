#ifndef BASIC_DEFINE_HPP
#define BASIC_DEFINE_HPP

#include <stdint.h>
#include <sys/types.h>

/********************************************************************
 *                        DEFINE		                           *
 ********************************************************************/
#define		RESET_LINE			"\r\033[K"

/* Typedef for integet type */
typedef		uint8_t		u8;			/* Unsigned int 8 bits */
typedef		uint16_t 	u16;		/* Unsigned int 16 bits */	
typedef		uint32_t 	u32;		/* Unsigned int 32 bits */
typedef		uint64_t 	u64;		/* Unsigned int 64 bits */
typedef		int8_t		s8;			/* Signed int 8 bits */
typedef		int16_t		s16;		/* Signed int 16 bits */
typedef		int32_t		s32;		/* Signed int 32 bits */
typedef		int64_t		s64;		/* Signed int 64 bits */

/* Typedef for float type */
typedef		float		f32;		/* Float 32 bits */
typedef		double		f64;		/* Float 64 bits */


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


/* Define RGBA color */
#define WHITE_RGBA			(RGBA_TO_UINT32(255,255,255,255))	/* Empty color */
#define GREEN_RGBA			(RGBA_TO_UINT32(0,255,0,255))		/* Snake head color */
#define DARK_GREEN_RGBA		(RGBA_TO_UINT32(0,100,0,255))		/* Snake body color */
#define YELLOW_RGBA			(RGBA_TO_UINT32(255,255,0,255))		/* Food color */
#define DARL_BLUE_RGBA		(RGBA_TO_UINT32(0,0,139,255))		/* Top Band color */
#define PINK_RGBA			(RGBA_TO_UINT32(255,0,255,255))		/* Snake head color */
#define DARK_RGBA			(RGBA_TO_UINT32(0,0,0,255))			/* Base color */
#define LIGHT_DARK_RGBA		(RGBA_TO_UINT32(50,50,50,150))		/* Base color */

/* Define tile size for the board */
#define TILE_SIZE 40
#define TILE_SPACING 2

/* Define a 2D vector */
typedef struct iVec2_s {
	s32 x;
	s32 y;
} iVec2;

/* Define button enum */
typedef enum {
	BTN_UNPRESS = 0U,
	BTN_PRESSED = 1U
} ButtonState;

typedef enum {
	BTN_RESUME = 0U,
	BTN_RESTART = 1U,
	BTN_MODE = 2U,
	BTN_QUIT = 3U
} ButtonId;


#endif /* BASIC_DEFINE_HPP */