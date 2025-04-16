NAME = philo

SRC = philo_utils.c philo.c utils.c

INC = philo.h
OBJS = ${SRC:.c=.o}
CC = cc -Wall -Werror -Wextra # -g3 -fsanitize=thread
RM = rm -f

all: ${NAME}

%.o: %.c ${INC}
	${CC} -c $< -o $@

${NAME}: ${OBJS}
	${CC} ${OBJS} -o ${NAME}

clean:
	${RM} ${OBJS} ${OBJS_BNS}

fclean: clean
	${RM} ${NAME} ${NAME_BNS}

re: fclean all