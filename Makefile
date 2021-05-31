NAME = minitalk
SERVER_NAME = server
CLIENT_NAME = client
CC = gcc
CFLAGS = -Werror -Wall -Wextra -g
SERVER_SRCS = server.c
CLIENT_SRCS = client.c

CLIENT_OBJS = $(CLIENT_SRCS:.c=.o)
SERVER_OBJS = $(SERVER_SRCS:.c=.o)

all: $(NAME)

$(NAME): $(SERVER_NAME) $(CLIENT_NAME)

SERVER_NAME:
	$(CC) $(CFLAGS) -o $(SERVER_NAME) $(SERVER_OBJS)

CLIENT_NAME:
	$(CC) $(CFLAGS) -o $(CLIENT_NAME) $(CLIENT_OBJS)

clean:

fclean: clean

re: fclean all

.PHONY: all clean fclean re
