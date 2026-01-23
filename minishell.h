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

typedef struct s_env
{
	char			*var;
	struct s_env	*next;
}			t_env;

typedef struct s_mshell_data
{
	t_env 		*env_var;
	t_tokens	*tokens;
}			t_mshell_data;

// Banner
void	print_banner(void);

//Lexing
void	create_tokens(char *prompt, t_tokens **tokens);
int		handle_quotes(t_tokens **tokens, char *str, char quote, int start);
void	add_token(t_tokens **tokens, char *input, t_node_type type);

// Input reading
void	handle_input(char *prompt);
int		check_exit (char *prompt);
void	add_type(t_tokens **tokens);

// Utils and list functions
void	list_add(t_tokens **tokens, char *input);
void	initialize(t_mshell_data **data, char **envp);
void	free_list(t_tokens *tokens);
char	**free_array(char **array, int n);
void	print_list(t_tokens *tokens);
char	**array_join(t_tokens **tokens);
void	create_envp(char **vars, t_mshell_data *data);
void	envp_add(t_mshell_data *data, char *input);
int		count_array(char **array);
void	print_env(t_mshell_data *data);
void	run_command(char **commandline, t_mshell_data *data);
t_env*	ft_env_var(t_env *env_list, char *var_name);



// Built ins
int		pwd();
int		env(char **commandline, t_mshell_data *data);
int		echo(char **arg);
int		cd(t_mshell_data *data, char **args);
/* void	export(char *commandline, t_mshell_data *data); */
void	ft_set_env_var(t_env **env_list, char *var_name, char *value);


// Parsing
void check_command(t_mshell_data *data);

#endif