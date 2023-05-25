# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cbernot <cbernot@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/04 13:41:24 by cbernot           #+#    #+#              #
#    Updated: 2023/05/24 18:40:06 by cbernot          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
SRCS = ./srcs/main.c ./srcs/init_params.c ./srcs/ft_atoi.c ./srcs/utils.c ./srcs/philo_routine.c \
		./srcs/death_routine.c
# 	 ./srcs/init_philo.c \
#     ./srcs/death_checker_routine.c ./srcs/utils2.c \

OBJS = ${SRCS:.c=.o}
INCLUDES = ./includes
FLAGS = #-Wall -Werror -Wextra -pthread
# -fsanitize=thread -g -ltsan

%.o : %.c ./includes/philo.h Makefile
	cc ${FLAGS} -I ${INCLUDES} -c $< -o $@

${NAME}: ${OBJS}
	cc ${OBJS} -pthread -o ${NAME}
#-ltsan

all: ${NAME}

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re