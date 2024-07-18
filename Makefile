include rsc/mk/color.mk
include rsc/mk/source.mk

NAME            =   nibbler
CC              =   g++
LIB_DIR		 	=   rsc/lib

CFLAGS     +=   -I./rsc/lib/SFML/SFML-2.6.1/include
LDFLAGS := -L./rsc/lib/SFML/SFML-2.6.1/lib -Wl,-rpath=./rsc/lib/SFML/SFML-2.6.1/lib
LDLIBS := -lsfml-graphics -lsfml-window -lsfml-system

all:        $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS)
	@$(MAKE_LIBFT)
	@$(MAKE_LIST)
	@printf "$(CYAN)Compiling ${NAME} ...$(RESET)\n"
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(NAME) $(OBJS) -lm $(LDLIBS)
	@printf "$(GREEN)Compiling $(NAME) done$(RESET)\n"


$(OBJ_DIR):
	@mkdir -p $(ALL_SRC_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@printf "$(YELLOW)Compile $<$(RESET) $(BRIGHT_BLACK)-->$(RESET) $(BRIGHT_MAGENTA)$@$(RESET)\n"
	@$(CC) $(CFLAGS) -o $@ -c $<

bonus: clear_mandatory $(NAME)

clear_mandatory:
ifeq ($(shell [ -f ${OBJ_DIR}/main.o ] && echo 0 || echo 1), 0)
	@printf "$(RED)Clean mandatory obj $(RESET)\n"
	@rm -rf ${OBJ_DIR}
endif

clean_lib:
	@rm -rf $(LIB_DIR)

clean:
ifeq ($(shell [ -d ${OBJ_DIR} ] && echo 0 || echo 1), 0)
	@$(RM) $(OBJ_DIR)
	@printf "$(RED)Clean $(OBJ_DIR) done$(RESET)\n"
	@$(RM)
endif

fclean:	 clean
	@$(RM) $(NAME)
	@printf "$(RED)Clean $(NAME)$(RESET)\n"

test: $(NAME)
	@./$(NAME)

# @ulimit -c unlimited
leak thread debug: clean $(NAME)
	@printf	"$(CYAN)CFLAGS: $(CFLAGS)$(RESET)\n"
# @./$(NAME)

re: clean $(NAME)

.PHONY:		all clean fclean re bonus" > Makefile
