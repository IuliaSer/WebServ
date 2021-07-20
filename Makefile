NAME = webserv

SRCS = main.cpp Parser.cpp Cgi.cpp Response.cpp

OBJS = $(SRCS:.c=.o)

CC = clang++

CFLAGS = -Wall -Wextra -Werror -g

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) -g

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	/bin/rm -f *.o

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
