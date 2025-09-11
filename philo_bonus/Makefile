NAME = philo_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

SRCDIR = src
INCDIR = includes

SRCS = $(SRCDIR)/main.c \
       $(SRCDIR)/utils.c \
       $(SRCDIR)/init.c \
       $(SRCDIR)/routine.c \
       $(SRCDIR)/monitor.c \
       $(SRCDIR)/time.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re