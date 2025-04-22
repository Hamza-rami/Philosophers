NAME = philo_bonus

SRC = philo_bonus.c utils.c routine.c philo_utils.c

INC = philo_bonus.h
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