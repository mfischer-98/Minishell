# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/07 11:08:27 by mefische          #+#    #+#              #
#    Updated: 2025/10/07 11:08:27 by mefische         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC = main.c exit_process/check_exit.c parsing/input_parsing.c parsing/parsing_utils.c banner.c builtins/pwd.c

OBJ = $(SRC:.c=.o)

CC = cc
CFLAGS = -Werror -Wextra -Wall -g

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) -lreadline

%.o: %.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -f $(OBJ)
		$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
		rm -f $(NAME)
		$(MAKE) -C $(LIBFT_DIR) fclean

valgrind: 
	@echo "{\n   leak readline\n   Memcheck:Leak\n...\n   fun:readline\n}\n{\n   leak add_history\n   Memcheck:Leak\n...\n   fun:add_history\n}" > readline.supp
	@valgrind --suppressions=readline.supp --leak-check=full -s --show-leak-kinds=all --track-fds=yes ./$(NAME)

re: fclean all