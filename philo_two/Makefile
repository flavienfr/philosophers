SRCS = main.c thread.c print_free.c utils.c

INC = -Iincludes/

NAME = philo_two

OBJS = ${SRCS:.c=.o}

FLAGS = -Wall -Wextra -Werror

all : $(NAME)

$(NAME) : ${OBJS}
		gcc ${OBJS} -o $(NAME) -lpthread

.c.o : ${SRCS}
		gcc ${INC} ${FLAGS} -c $< -o ${<:.c=.o}

clean :
		rm -f ${OBJS}

fclean : clean
		rm -f $(NAME)

re : fclean
		make all

.PHONY: all clean fclean re
