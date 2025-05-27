NAME := pipex
SRC := main.c path.c util.c
OBJ := $(SRC:.c=.o)
DEP := $(SRC:.c=.d)
CFLAGS := -Wall -Wextra -Werror -MMD -MP

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $^ -o $@

clean:
	$(RM) $(OBJ) $(DEP)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
.SECONDARY: $(OBJ) $(DEP)

-include $(DEP)
