NAME = phillo
NAME_BNS = phillo_bonus

SRC = philo/philo_utils.c philo/philo.c philo/utils.c philo/monitor.c
SRC_BNS = philo_bonus/philo_bonus.c philo_bonus/utils.c

INC = philo/philo.h
INC_BNS = philo_bonus/philo_bonus.h
OBJS = ${SRC:.c=.o}
OBJS_BNS = ${SRC_BNS:.c=.o}
CC = cc -Wall -Werror -Wextra
RM = rm -f

all: ${NAME}

philo/%.o: philo/%.c ${INC}
	${CC} -c $< -o $@

philo_bonus/%.o: philo_bonus/%.c ${INC_BNS}
	${CC} -c $< -o $@

${NAME}: ${OBJS}
	${CC} ${OBJS} -o ${NAME}

bonus: ${OBJS_BNS}
	${CC} ${OBJS_BNS} -o ${NAME_BNS}

clean:
	${RM} ${OBJS} ${OBJS_BNS}

fclean: clean
	${RM} ${NAME} ${NAME_BNS}

re: fclean all