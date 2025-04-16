NAME = phillo

SRC = philo/philo_utils.c philo/philo.c philo/utils.c philo/monitor.c

INC = philo/philo.h
OBJS = ${SRC:.c=.o}
CC = cc -Wall -Werror -Wextra # -g3 -fsanitize=thread
RM = rm -f

all: ${NAME}

philo/%.o: philo/%.c ${INC}
	${CC} -c $< -o $@

${NAME}: ${OBJS}
	${CC} ${OBJS} -o ${NAME}

clean:
	${RM} ${OBJS} ${OBJS_BNS}

fclean: clean
	${RM} ${NAME} ${NAME_BNS}

re: fclean all