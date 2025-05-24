NAME := pipex
SRC := main.c
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

test: all
	./pipex Makefile "grep clean" "wc -l" output.txt
	cat output.txt
	rm -f output.txt

.PHONY: all clean fclean re run test
.SECONDARY: $(OBJ) $(DEP)
.SILENT:

-include $(DEP)
