# Variables
CC = cc               # Compiler
CFLAGS = -Wextra -Werror -Wall -pthread  # Compiler flags
NAME = philo_one           # Name of the output executable
SRCS = philo.c          # Source files
OBJS = $(SRCS:.c=.o)   # Object files

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
