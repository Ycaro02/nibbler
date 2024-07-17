include rsc/mk/color.mk
include rsc/mk/source.mk

NAME            =   nibbler
CC              =   g++

all:        $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS)
	@$(MAKE_LIBFT)
	@$(MAKE_LIST)
	@printf "$(CYAN)Compiling ${NAME} ...$(RESET)\n"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lm
	@printf "$(GREEN)Compiling $(NAME) done$(RESET)\n"


$(OBJ_DIR):
	@mkdir -p $(ALL_SRC_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@printf "$(YELLOW)Compile $<$(RESET) $(BRIGHT_BLACK)-->$(RESET) $(BRIGHT_MAGENTA)$@$(RESET)\n"
	@$(CC) $(CFLAGS) -o $@ -c $<

bonus: clear_mandatory $(NAME)

clear_mandatory:
ifeq ($(shell [ -f ${OBJ_DIR}/main.o ] && echo 0 || echo 1), 0)
	@printf "$(RED)Clean mandatory obj $(RESET)\n"
	@rm -rf ${OBJ_DIR}
endif

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
