# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/23 13:50:53 by jyniemit          #+#    #+#              #
#    Updated: 2025/10/28 13:01:05 by jyniemit         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

OBJDIR = obj

SRCS = main.c \
       utils.c \
       init.c \
       time.c \
	   print.c \
       monitor.c \
       forks.c \
       philo_routine.c \
       philo_routine_utils.c \
       mutex.c \

HEADER = philo.h

OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(OBJDIR)/%.o: %.c $(HEADER) | $(OBJDIR) 
	$(CC) $(CFLAGS) -I. -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	$(RM) -r $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

debug: fclean
	$(MAKE) CFLAGS="$(CFLAGS) -g" all

re: fclean all

.PHONY: all clean fclean re debug
.SECONDARY: $(OBJS) $(OBJDIR)
