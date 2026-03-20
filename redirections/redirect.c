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
static int	apply_input(t_tokens *token, t_mshell_data *data)
{
	int		fd;
	char	*file;

	file = strip_file_quotes(token->redir_file);
	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (free(file), check_fd_error(token->redir_file, data));
	if (dup2(fd, STDIN_FILENO) == -1)
		return (perror("dup2"), close(fd), free(file), 1);
	close(fd);
	free(file);
	return (0);
}

/* Apply output: > file -> stdout truncate */
static int	apply_output(t_tokens *token, t_mshell_data *data)
{
	int		fd;
	char	*file;

	file = strip_file_quotes(token->redir_file);
	fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (free(file), check_fd_error(token->redir_file, data));
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (perror("dup2"), close(fd), free(file), 1);
	close(fd);
	free(file);
	return (0);
}

/* Apply append: >> file -> stdout append */
static int	apply_append(t_tokens *token, t_mshell_data *data)
{
	int		fd;
	char	*file;

	file = strip_file_quotes(token->redir_file);
	fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (fd < 0)
		return (free(file), check_fd_error(token->redir_file, data));
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (perror("dup2"), close(fd), free(file), 1);
	close(fd);
	free(file);
	return (0);
}

/* Apply redirections in child process 
	- dup2 makes new fd out of the temp fd,
	so file is read instead of keyboard
*/
int	apply_redirects(t_tokens *tokens, t_mshell_data *data)
{
	t_tokens	*temp;
	int			result;

	result = 0;
	temp = tokens;
	while (temp && temp->type != NODE_PIPE)
	{
		if (temp->type == NODE_HERE && temp->redir_file)
			result = apply_heredoc(temp->heredoc_fd);
		else if (temp->type == NODE_IN && temp->redir_file)
			result = apply_input(temp, data);
		else if (temp->type == NODE_OUT && temp->redir_file)
			result = apply_output(temp, data);
		else if (temp->type == NODE_APPEND && temp->redir_file)
			result = apply_append(temp, data);
		if (result != 0)
			return (result);
		temp = temp->next;
	}
	return (0);
}
