SRCS = main.c philo.c print.c close.c utils.c other.c

INC = -Iincludes/

NAME = philo_three

OBJS = ${SRCS:.c=.o}

FLAGS = -Wall -Wextra -Werror

all : $(NAME)

$(NAME) : ${OBJS}
		gcc ${OBJS} -o $(NAME) -lpthread # useless -lpthread 

.c.o : ${SRCS}
		gcc ${INC} ${FLAGS} -c $< -o ${<:.c=.o}

clean :
		rm -f ${OBJS}

fclean : clean
		rm -f $(NAME)

re : fclean
		make all

.PHONY: all clean fclean re
