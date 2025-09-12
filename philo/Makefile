# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/23 13:50:53 by jyniemit          #+#    #+#              #
#    Updated: 2025/09/12 19:30:00 by jyniemit         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

SRCDIR = src
INCDIR = includes

SRCS = $(SRCDIR)/main.c \
       $(SRCDIR)/utils.c \
       $(SRCDIR)/init.c \
       $(SRCDIR)/time.c \
       $(SRCDIR)/monitor.c \
       $(SRCDIR)/output.c \
       $(SRCDIR)/system.c \
       $(SRCDIR)/philo_routine.c

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

debug: fclean
	$(MAKE) CFLAGS="$(CFLAGS) -g" all

re: fclean all

.PHONY: all clean fclean re
