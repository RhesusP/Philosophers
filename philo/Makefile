# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/04 13:41:24 by cbernot           #+#    #+#              #
#    Updated: 2023/06/14 11:53:12 by cbernot          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
SRCS =  ./srcs/main.c \
		./srcs/init_params.c \
		./srcs/ft_atoi.c \
		./srcs/input_checker.c \
		./srcs/philo_routine.c \
		./srcs/utils.c \
		./srcs/death_routine.c 

OBJS = ${SRCS:.c=.o}
INCLUDES = ./includes
FLAGS = -Wall -Werror -Wextra -pthread

%.o : %.c ./includes/philo.h Makefile
	gcc ${FLAGS} -I ${INCLUDES} -c $< -o $@

${NAME}: ${OBJS}
	gcc ${OBJS} -pthread -o ${NAME} 

all: ${NAME}

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re
