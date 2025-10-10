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

typedef struct s_commands
{
	char	*input;
	char	type;
	struct s_commands *next;
}			t_commands;

// Banner
void	print_banner(void);

// Input reading
void	handle_input(char *prompt);
int		check_exit (char *prompt);

// Utils and list functions
void	list_add(t_commands **commands, char *input);
void	free_list(t_commands *commands);
char	**free_array(char **array, int n);

#endif