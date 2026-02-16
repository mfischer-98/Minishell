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
# define MINISHELL_H

# include <stdlib.h>
# include "libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>

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

typedef struct s_token_state
{
	int		i;
	int		start;
	int		in_quote;
	char	quote_char;
}			t_token_state;

typedef struct s_tokens
{
	char			*input;
	t_node_type		type;
	struct s_tokens	*next;
}			t_tokens;

typedef struct s_env
{
	char			*var;
	struct s_env	*next;
}			t_env;

typedef struct s_expander
{
	char	*result;
	int		i;
	int		in_double;
	int		in_single;
}			t_expander;

typedef struct s_mshell_data
{
	t_env		*env_var;
	t_tokens	*tokens;
	t_expander	*expander;
	int			exit_status;
}			t_mshell_data;

// Banner
void	print_banner(void);

//Lexing
void	create_tokens(char *prompt, t_tokens **tokens);
void	add_token(t_tokens **tokens, char *input, t_node_type type);
void	handle_space(t_tokens **tokens, char *prompt, t_token_state *state);
void	handle_quote_end(t_tokens **tokens, char *prompt, t_token_state *state);
void	handle_quote_start(char *prompt, t_token_state *state);
void	handle_tok_type(t_tokens **tokens, char *prompt, t_token_state *state);
void	add_type(t_tokens **tokens);

// Input reading
void	handle_input(char *prompt);

// Parser Expander
void	check_command(t_mshell_data *data);
char	*get_env_var(char *token, t_mshell_data *data);
char	*expand_tokens(char *token, t_mshell_data *data);
void	set_operator_type(t_tokens *token);
void	set_quote_type(t_tokens *token);
char	*handle_brackets(char	*str, int len, int start, t_mshell_data *data);
void	var_name_error(char *input, t_mshell_data *data);
void	append_result(char	*str, t_mshell_data *data);

// Executor
void	executor(t_mshell_data *data);

// Utils and list functions
void	list_add(t_tokens **tokens, char *input);
int		array_size(t_tokens *tokens);
void	initialize(t_mshell_data **data, char **envp);
void	init_expander(t_expander **expander);
t_token_state	*init_state(void);
void	free_list(t_tokens *tokens);
void	free_array(char **array, int n);
void	free_data(t_mshell_data *data);
void	free_env(t_env *env);
void	print_list(t_tokens *tokens);
char	**array_join(t_tokens *tokens);
void	create_envp(char **vars, t_mshell_data *data);
void	envp_add(t_mshell_data *data, char *input);
int		env_size(t_env *env_list);
void	print_env(t_mshell_data *data);
void	run_builtin(char **commandline, t_mshell_data *data);
t_env	*ft_env_var(t_env *env_list, char *var_name);

// Built ins
int		pwd(void);
int		env(char **commandline, t_mshell_data *data);
int		echo(char **arg);
int		cd(t_mshell_data *data, char **args);
int		check_exit(char **command, t_mshell_data *data);
// export
int		export(char **commandline, t_mshell_data *data);
int		handle_no_equal(char *arg, t_mshell_data *data);
int		handle_equal(char *arg, t_mshell_data *data);
int		process_export(char *commandline, t_mshell_data *data);
int		identifier_valid(char *str);
char	**list_to_array(t_env *env_list, int size);
void	bubble_sort_export(char **env_array, int size);
int		print_export(t_mshell_data *data);
int		check_env_list(char *str, t_mshell_data *data);
int		add_env_list(char *str, t_mshell_data *data);
int		update_env_list(char *str, t_mshell_data *data);
char	*trim_outer_quotes(char	*str);
int		get_name_len(char	*str);
char	*get_append_value(char	*str);
char	*get_old_var(char *name, int len, t_mshell_data *data);
char	*build_var(char *name, char *old_var, char *new_value);
int		handle_append(char *str, t_mshell_data *data);
// unset
void	unset_env(char *str, t_mshell_data *data);
int		unset(char **commandline, t_mshell_data *data);

#endif