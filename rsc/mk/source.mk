CFLAGS			=	-Wall -Wextra -Werror -O3

OBJ_DIR			=	obj

ALL_SRC_DIR 	=	obj\
					$(OBJ_DIR)/SFML\
					$(OBJ_DIR)/SDL2\
					$(OBJ_DIR)/GraphicLib\
					$(OBJ_DIR)/Menu\

SRC_DIR 		=	src

MAIN_MANDATORY 	=	main.cpp

SRCS			=	GraphicLib/GraphicLib.cpp\
					Nibbler.cpp \
					Snake.cpp \
					Menu/Menu.cpp \
					Menu/Button.cpp \

# SRCS_BONUS		=	main_bonus.cpp

OBJS 			= $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

RM			=	rm -rf

ifeq ($(findstring bonus, $(MAKECMDGOALS)), bonus)
ASCII_NAME	= "bonus"
SRCS += $(SRCS_BONUS)
else
SRCS += $(MAIN_MANDATORY)
endif

ifeq ($(findstring leak, $(MAKECMDGOALS)), leak)
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address
else ifeq ($(findstring thread, $(MAKECMDGOALS)), thread)
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=thread
else ifeq ($(findstring debug, $(MAKECMDGOALS)), debug)
CFLAGS = -Wall -Wextra -Werror -g3
endif
