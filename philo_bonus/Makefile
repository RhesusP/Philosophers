# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/04 13:41:24 by cbernot           #+#    #+#              #
#    Updated: 2023/02/22 18:48:13 by cbernot          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus
SRCS = ./srcs/main.c ./srcs/ft_atoi.c ./srcs/philo_routine.c ./srcs/utils.c \
		./srcs/death_checker_routine.c ./srcs/init_philo.c 
OBJS = ${SRCS:.c=.o}
INCLUDES = ./includes
FLAGS = -Wall -Werror -Wextra -pthread

%.o : %.c ./includes/philo_bonus.h
	cc ${FLAGS} -I ${INCLUDES} -c $< -o $@

${NAME}: ${OBJS}
	cc ${OBJS} -pthread -o ${NAME}

all: ${NAME}

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re