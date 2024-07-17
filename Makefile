NAME = philo
CC = cc
FLAGS = -Wall -Wextra -Werror -g #-fsanitize=thread
PFLAGS = -lpthread
SRCS =	philo.c \
		utils.c \
		init.c \
		errorchecks.c \
		begin.c \
		routines.c
INCLUDES = -I./
OBJS = $(SRCS:.c=.o)
RM = rm -rf

all : $(NAME)

$(NAME): $(OBJS)
	$(CC) $(PFLAGS) $(FLAGS) $(OBJS) -o $(NAME)

%.o : %.c
	$(CC) $(FLAGS)  $(INCLUDES) -c $< -o $@

clean :
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re : fclean all

.PHONY: all clean fclean re
