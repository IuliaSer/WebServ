NAME = webserv

SRCS = main.cpp Request.cpp Cgi.cpp Response.cpp Sockets.cpp Config.cpp Server.cpp Location.cpp Utils.cpp

OBJS = $(SRCS:.c=.o)

CC = clang++

CFLAGS = -Wall -Wextra -Werror -g

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	/bin/rm -f *.o

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
