NAME = philo
CC = cc
FLAGS = -Wall -Wextra -Werror -lpthread #-fsanitize=thread
SRCS = parser.c \
		utils.c \
		philo.c \
		threads.c \
		errorchecks.c 
INCLUDES = -I./
OBJS = $(SRCS:.c=.o)
RM = rm -rf

all : $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

%.o : %.c
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

clean :
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re : fclean all

.PHONY: all clean fclean re
