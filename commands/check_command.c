/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 11:08:43 by mefische          #+#    #+#             *//*   Updated: 2025/10/22 15:24:10 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void run_command(char **commandline, char **envp)
{
	if (!ft_strcmp(commandline[0], "pwd"))
		pwd();
	// else if (!ft_strcmp(command, "cd"))
	// 	cd();
	// else if (!ft_strcmp(command, "env"))
	// 	env();
	// else if (!ft_strcmp(command, "echo"))
	// 	echo();
	// else if (!ft_strcmp(command, "export"))
	// 	export();
	// else if (!ft_strcmp(command, "unset"))
	// 	unset();
	else
		execve(ft_strjoin(getcwd(NULL, 0), "./minishell"), commandline, envp);
}

void check_command(t_tokens **tokens, char **envp)
{
	t_tokens	*temp;

	(void)envp;
	temp = *tokens;
	if (temp->type == NODE_WORD)
		run_command(array_join(tokens), envp);
	//else if (temp->type == NODE_HERE)
	else
		ft_printf("Error?\n");
	// while (temp)
	// {
	// 	if (temp->type == NODE_PIPE)
	// 		check_command(&temp);
	// 	temp = temp->next;
	// } 
}
