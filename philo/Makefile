NAME = philo

SRC = philo_utils.c philo.c utils.c monitor.c

INC = philo.h
OBJS = ${SRC:.c=.o}
CC = cc -Wall -Werror -Wextra
RM = rm -f

all: ${NAME}
%.o: %.c ${INC}
	${CC} -c $< -o $@

${NAME}: ${OBJS}
	${CC} ${OBJS} -o ${NAME}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all