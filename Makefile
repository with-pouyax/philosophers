# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/19 14:00:00 by pouyaximac        #+#    #+#              #
#    Updated: 2024/11/19 10:45:34 by pghajard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= philo

CC		= gcc
CFLAGS	= -Wall -Wextra -Werror -pthread

SRCS	= main.c \
		  init.c \
		  validation.c \
		  utils.c \
		  philo_utils.c \
		  philo_utils_extra.c \
		  philo_life.c \
		  monitor.c \
		  ft_atoll.c \
		  cleanup.c

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c philo.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
