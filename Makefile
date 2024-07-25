include rsc/mk/color.mk
include rsc/mk/source.mk

NAME            =   nibbler
CC              =   g++
LIB_DIR		 	=   rsc/lib

CFLAGS			+=	-I./rsc/lib/install/include/
# LDFLAGS			:=	-L./rsc/lib/SFML/build/lib  -Wl,-rpath=./rsc/lib/SFML/build/lib 
# LDLIBS			:=	-lsfml-graphics -lsfml-window -lsfml-system

WRAPPER_LIB_DIR	=	./rsc/wrapperlib
WRAPPER_COMPILE =	./rsc/sh/wrapper_compile.sh

LIB_DEPS		=	./rsc/lib

all:        $(NAME)

$(NAME): $(LIB_DEPS) $(WRAPPER_LIB_DIR) $(OBJ_DIR) $(OBJS) 
	@printf "$(CYAN)Compiling ${NAME} ...$(RESET)\n"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@printf "$(GREEN)Compiling $(NAME) done$(RESET)\n"


$(OBJ_DIR):
	@mkdir -p $(ALL_SRC_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@printf "$(YELLOW)Compile $<$(RESET) $(BRIGHT_BLACK)-->$(RESET) $(BRIGHT_MAGENTA)$@$(RESET)\n"
	@$(CC) $(CFLAGS) -o $@ -c $<

$(LIB_DEPS) :
ifeq ($(shell [ -d $(LIB_DEPS) ] && echo 0 || echo 1), 1)
	@./rsc/install/load_lib.sh
endif

$(WRAPPER_LIB_DIR) :
ifeq ($(shell [ -d $(WRAPPER_LIB_DIR) ] && echo 0 || echo 1), 1)
	@$(WRAPPER_COMPILE) SFMLWrapper.so src/SFML/SF_C_Wrapper.cpp "-lsfml-graphics -lsfml-window -lsfml-system"
	@$(WRAPPER_COMPILE) SDL2Wrapper.so src/SDL2/SDL_C_Wrapper.cpp "-lSDL2"
	@$(WRAPPER_COMPILE) RaylibWrapper.so src/Raylib/Raylib_C_Wrapper.cpp "-lraylib"
endif

bonus: clear_mandatory $(NAME)

clear_mandatory:
ifeq ($(shell [ -f $(OBJ_DIR)/main.o ] && echo 0 || echo 1), 0)
	@printf "$(RED)Clean mandatory obj $(RESET)\n"
	@rm -rf $(OBJ_DIR)
endif

clean_lib:
	@rm -rf $(LIB_DIR)

clean:
ifeq ($(shell [ -d $(OBJ_DIR) ] && echo 0 || echo 1), 0)
	@$(RM) $(OBJ_DIR)
	@printf "$(RED)Clean $(OBJ_DIR) done$(RESET)\n"
	@$(RM)
endif

fclean:	 clean
	@$(RM) $(NAME) $(WRAPPER_LIB_DIR)
	@printf "$(RED)Clean $(NAME)$(RESET)\n"

test: $(NAME) $(WRAPPER_LIB_DIR)
	@./$(NAME) 20 20

# @ulimit -c unlimited
leak thread debug: clean $(NAME)
	@printf	"$(CYAN)CFLAGS: $(CFLAGS)$(RESET)\n"
# @./$(NAME)

re: clean $(NAME)

.PHONY:		all clean fclean re bonus" > Makefile
