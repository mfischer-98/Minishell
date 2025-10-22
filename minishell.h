/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 11:08:18 by mefische          #+#    #+#             */
/*   Updated: 2025/10/07 11:08:18 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

# include <stdlib.h>
# include "libft/libft.h"
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <readline/readline.h>
#include <readline/history.h>

//COLORS
# define CYAN "\033[96m"
# define YELLOW "\033[93m"
# define GREEN "\033[92m"
# define MAGENTA "\033[95m"
# define BLUE "\033[94m"
# define RED "\033[91m"
# define RESET "\033[0m"

typedef enum s_node_type
{
	NODE_UNKNOWN,
	NODE_WORD,
	NODE_PIPE,
	NODE_OUT, //REDIRECT
	NODE_IN,
	NODE_APPEND,
	NODE_HERE,
	NODE_SINGLE_QUOTE,
	NODE_DOUBLE_QUOTE,
}		t_node_type;

typedef struct s_tokens
{
	char		*input;
	t_node_type	type;
	struct s_tokens *next;
}			t_tokens;

// Banner
void	print_banner(void);

//Lexing
void	create_tokens(char *prompt);
int		handle_quotes(t_tokens **tokens, char *str, char quote, int start);
void	add_token(t_tokens **tokens, char *input, t_node_type type);

// Input reading
void	handle_input(char *prompt);
int		check_exit (char *prompt);
void	add_type(t_tokens **tokens);

// Utils and list functions
void	list_add(t_tokens **tokens, char *input);
void	free_list(t_tokens *tokens);
char	**free_array(char **array, int n);
void	print_list(t_tokens *tokens);

//built ins
void	pwd();
void	env();

#endif