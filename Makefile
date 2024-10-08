NAME = philo
CC = cc
FLAGS = -Wall -Wextra -Werror -pthread #-fsanitize=thread
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
	$(CC) $(FLAGS) $(OBJS) -o $(NAME) $(INCLUDES)

%.o : %.c
	$(CC) -c $(FLAGS) $< -o $@

clean :
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re : fclean all

.PHONY: all clean fclean re
