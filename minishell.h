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

typedef struct s_commands
{
	char	*input;
	char	type;
	struct s_commands *next;
}			t_commands;

int		check_exit (char *prompt);
void	handle_input(char *prompt);
void	list_add(t_commands **commands, char *input);
char	**free_array(char **array, int n);
void	free_list(t_commands *commands);

#endif