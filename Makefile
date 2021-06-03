NAME = minitalk
SERVER_NAME = server
CLIENT_NAME = client
CC = gcc
CFLAGS = -Werror -Wall -Wextra -g
SERVER_HEADERS = server.h
SERVER_SRCS := server.c server_sighandlers.c
CLIENT_SRCS = client.c
CLIENT_OBJS = $(CLIENT_SRCS:.c=.o)
SERVER_OBJS = $(SERVER_SRCS:.c=.o)
SERVER_DEPS = $(SERVER_SRCS:.c=.d)

all: $(NAME)

$(NAME): $(SERVER_NAME) $(CLIENT_NAME)

%.o: %.c
	# -MMD -MP -MF で依存ファイルリスト生成
	$(CC) $(CFLAGS)  -MMD -MP -MF $(<:.c=.d) -c $< -o $@

-include $(SERVER_DEPS)
$(SERVER_NAME): $(SERVER_OBJS) $(SERVER_HEADERS)
	$(CC) $(CFLAGS) -o $(SERVER_NAME) $(SERVER_OBJS)

$(CLIENT_NAME): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $(CLIENT_NAME) $(CLIENT_OBJS)

clean:
	${RM} ${SERVER_OBJS}
	${RM} ${CLIENT_OBJS}
	${RM} ${SERVER_DEPS}

fclean: clean
	${RM} ${SERVER_NAME}
	${RM} ${CLIENT_NAME}

re: fclean all

.PHONY: all clean fclean re
