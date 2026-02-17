/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 13:36:30 by mefische          #+#    #+#             */
/*   Updated: 2026/02/17 13:36:30 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_redirect(t_mshell_data	*data)
{
	t_tokens	*temp;
	
	temp = data->tokens;
	while (temp)
	{
		if (temp->type == NODE_OUT || temp->type == NODE_IN
				|| temp->type == NODE_APPEND || temp->type == NODE_HERE)
				return (1)
		temp = temp->next;
	}
	return (0);
}

void	check_redir_type(char **commandline, t_mshell_data	*data)
{
	t_env	*temp;
	
	temp = data->env_var;
	while (temp)
	{
		if (temp->type == NODE_HERE)
			handle_heredoc(commandline, data);
		temp = temp->next;
	}
	return (0);
}

/* Apply redirections in child process */


// Apply input
// Apply output
// Apply append
// Apply heredoc






