#ifndef SHORT_TYPE_HPP
#define SHORT_TYPE_HPP

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

/* Define a 2D vector */
typedef struct iVec2_s {
	s32 x;
	s32 y;
} iVec2;

#endif /* SHORT_TYPE_HPP */