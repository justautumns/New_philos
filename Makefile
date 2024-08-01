NAME = philo
CC = cc
FLAGS = -Wall -Wextra -Werror -g #-fsanitize=thread
LIBS = -lpthread
SRCS =	philo.c \
		utils.c \
		init.c \
		errorchecks.c \
		begin.c \
		routines.c \
		sauron.c
INCLUDES = -I./
HEADER = philo.h
OBJS = $(SRCS:.c=.o)
RM = rm -rf

all : $(NAME)

$(NAME): $(OBJS) $(HEADER)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME) $(INCLUDES) $(LIBS)

%.o : %.c
	$(CC) -c $(FLAGS) $< -o $@

clean :
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re : fclean all

.PHONY: all clean fclean re
