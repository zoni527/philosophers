# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/21 10:37:20 by jvarila           #+#    #+#              #
#    Updated: 2025/02/26 16:06:29 by jvarila          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= philo

CC		:= cc
CFLAGS		:= -Wall -Wextra -Werror
DEBUGFLAGS	:= -g
#------------------------------------------------------------------------------#
SRCDIR		:= ./
INCDIR		:= ./
OBJDIR		:= ./obj
#------------------------------------------------------------------------------#
SRC	:=	philo_main.c		\
		philo_data_setup.c	\
		philo_activities.c	\
		philo_time_management.c	\
		philo_input.c		\
		philo_output.c		\
		philo_cleanup.c		\
		philo_utils.c

OBJ		:= $(addprefix $(OBJDIR)/, $(SRC:.c=.o))
#------------------------------------------------------------------------------#
PHILOH		:= $(INCDIR)/philo.h
INC		:= -I $(INCDIR)
#------------------------------------------------------------------------------#
all: $(NAME)

$(NAME): $(OBJDIR) $(OBJ) $(PHILOH)
	$(CC) $(CFLAGS) $(OBJ) $(INC) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(PHILOH)
	$(CC) $(CFLAGS) -c $< $(INC) -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
#------------------------------------------------------------------------------#
debug: CFLAGS += $(DEBUGFLAGS)
debug: re
#------------------------------------------------------------------------------#
.PHONY: all clean fclean re debug libftdebug
#------------------------------------------------------------------------------#
