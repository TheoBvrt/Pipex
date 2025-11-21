NAME = pipex

SRCS =	./srcs/main.c \
		./srcs/utils.c \
		./srcs/parsing.c \
		./srcs/pipeline.c \
		./srcs/exit.c

OBJS = ${SRCS:.c=.o}
CC = gcc
RM = rm -f
CFLAGS = -Iheaders -Wall -Werror -Wextra -g

all : ${NAME}

%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@

${NAME} : ${OBJS}
	${MAKE} -C ./libft
	$(CC) $(OBJS) ./libft/libft.a -o $(NAME)

clean:
	${RM} ${OBJS}
	make clean -C ./libft

fclean: clean
	${RM} ${NAME}
	make fclean -C ./libft

re: fclean all
