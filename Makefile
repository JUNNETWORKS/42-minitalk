NAME = minitalk
SERVER_NAME = server
CLIENT_NAME = client
CC = gcc
CFLAGS = -Werror -Wall -Wextra -g
SERVER_HEADERS = server.h
SERVER_SRCS := server.c server_sighandlers.c
ifdef BONUS
	SERVER_SRCS := server_bonus.c server_sighandlers_bonus.c
endif
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

NAME_BONUS = minitalk_bonus
SERVER_NAME_BONUS = server_bonus
CLIENT_NAME_BONUS = client_bonus
SERVER_SRCS_BONUS = server_bonus.c server_sighandlers_bonus.c
SERVER_HEADERS_BONUS = server_bonus.h
CLIENT_SRCS_BONUS = client_bonus.c
CLIENT_OBJS_BONUS = $(CLIENT_SRCS_BONUS:.c=.o)
SERVER_OBJS_BONUS = $(SERVER_SRCS_BONUS:.c=.o)

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(SERVER_NAME_BONUS) $(CLIENT_NAME_BONUS)

$(SERVER_NAME_BONUS): $(SERVER_OBJS_BONUS)
	$(CC) $(CFLAGS) -o $(SERVER_NAME) $(SERVER_OBJS_BONUS)

$(CLIENT_NAME_BONUS): $(CLIENT_OBJS_BONUS)
	$(CC) $(CFLAGS) -o $(CLIENT_NAME) $(CLIENT_OBJS_BONUS)

clean:
	${RM} ${SERVER_OBJS}
	${RM} ${CLIENT_OBJS}
	${RM} ${SERVER_OBJS_BONUS}
	${RM} ${CLIENT_OBJS_BONUS}
	${RM} ${SERVER_DEPS}

fclean: clean
	${RM} ${SERVER_NAME}
	${RM} ${CLIENT_NAME}
	${RM} ${SERVER_NAME_BONUS}
	${RM} ${CLIENT_NAME_BONUS}

re: fclean all

.PHONY: all clean fclean re
