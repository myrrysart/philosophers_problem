# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/23 13:50:53 by jyniemit          #+#    #+#              #
#    Updated: 2025/10/21 15:39:33 by jyniemit         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

SRCDIR = src
INCDIR = includes
OBJDIR = obj

SRCS = $(SRCDIR)/main.c \
       $(SRCDIR)/utils.c \
       $(SRCDIR)/init.c \
       $(SRCDIR)/time.c \
       $(SRCDIR)/monitor.c \
       $(SRCDIR)/output.c \
       $(SRCDIR)/system.c \
       $(SRCDIR)/forks.c \
       $(SRCDIR)/philo_routine.c

OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	$(RM) -r $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

debug: fclean
	$(MAKE) CFLAGS="$(CFLAGS) -g" all

re: fclean all

.PHONY: all clean fclean re
