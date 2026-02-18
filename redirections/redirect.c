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

/* Apply redirections in child process 
	- dup2 makes new fd out of the temp fd,
	so file is read instead of keyboard
*/
int	apply_redirects(t_tokens *tokens)
{
	t_tokens	*temp;
	
	temp = tokens;
	while (temp && temp->type != NODE_PIPE)
	{
		if (temp->type == NODE_HERE)
		{
			if (temp->heredoc_fd < 0) //nao tem fd
				return (1);
			if (dup2(temp->heredoc_fd, STDIN_FILENO) == -1) //le o fd do heredoc
				return (perror("dup2"), 1);
			close(temp->heredoc_fd);
		}
		//add other redirects later
		temp = temp->next;
	}
	return (0);
}


// Apply input
// Apply output
// Apply append
// Apply heredoc