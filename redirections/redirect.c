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

/* Apply heredoc: << */
static int	apply_heredoc(int fd)
{
	if (fd < 0)
		return (1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (perror("dup2"), 1);
	close(fd);
	return (0);
}

/* Apply input: < file -> stdin */
static int	apply_input(char	*file)
{
	int	fd;
	
	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (perror("open <"), 1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (perror("dup2"), close(fd), 1);
	close(fd);
	return (0);
}

/* Apply output: > file -> stdout truncate */
static int	apply_output(char	*file)
{
	int	fd;
	
	fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (perror("open <"), 1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (perror("dup2"), close(fd), 1);
	close(fd);
	return (0);
}

/* Apply append: >> file -> stdout append */
static int	apply_append(char	*file)
{
	int	fd;
	
	fd = open(file,  O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (fd < 0)
		return (perror("open <"), 1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (perror("dup2"), close(fd), 1);
	close(fd);
	return (0);
}

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
		if (temp->type == NODE_HERE && apply_heredoc(temp->heredoc_fd))
			return (1);
		else if (temp->type == NODE_IN && apply_input(temp->redir_file))
			return (1);
		else if (temp->type == NODE_OUT && apply_output(temp->redir_file))
			return (1);
		else if (temp->type == NODE_APPEND && apply_append(temp->redir_file))
			return (1);
		temp = temp->next;
	}
	return (0);
}
