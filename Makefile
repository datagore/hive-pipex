NAME := pipex
SRC := main.c util.c exec.c
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
	valgrind --track-fds=yes ./pipex Makefile "grep clean" "wc -l" output.txt
	cat output.txt
	rm -f output.txt

hang: all
	valgrind --track-fds=yes ./pipex abc def "wc -l" output.txt
	cat output.txt
	rm -f output.txt

tester:
	cd tester && bash run.sh

.PHONY: all clean fclean re run test hang tester
.SECONDARY: $(OBJ) $(DEP)

-include $(DEP)
