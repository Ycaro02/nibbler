#ifndef SHORT_TYPE_HPP
#define SHORT_TYPE_HPP

#include <stdint.h>
#include <sys/types.h>

/********************************************************************
 *                        DEFINE		                           *
 ********************************************************************/
/* ANSI COLOR */
#define		RED     			"\x1b[31m"
#define		GREEN   			"\x1b[32m"
#define		YELLOW  			"\x1b[33m"
#define		BLUE    			"\x1b[34m"
#define		PURPLE  			"\x1b[35m"
#define		CYAN    			"\x1b[36m"
#define		PINK    			"\x1b[38;5;200m"
#define		ORANGE  			"\x1b[38;5;208m"
#define		RESET   			"\x1b[0m"
#define		FILL_GREEN			"\033[7;32m"
#define		FILL_YELLOW			"\033[7;33m"
#define		FILL_RED			"\033[7;31m"
#define		FILL_YELBLACK		"\033[40;33m"

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



#endif /* SHORT_TYPE_HPP */