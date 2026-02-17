/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 14:22:55 by mefische          #+#    #+#             */
/*   Updated: 2026/02/17 14:22:55 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Prepares heredocs before executor forks 
	- Need to know before child because heredoc reads from terminal (readline)
	- if I find a heredoc in tokens, handle heredoc
	- 1 = success and 0 = failure		*/
int	prep_heredoc(t_mshell_data *data)
{
	t_tokens	*temp;

	temp = data->tokens;
	while (temp)
	{
		if (temp->type == NODE_HERE)
		{
			if (handle_heredoc(temp, data) < 0)
			{
				data->exit_status = 1;
				return (0);
			}
		}
		temp = temp->next;
	}
	return (1);
}

/* Expand heredoc: expands variables in heredoc line */
char	*expand_heredoc_line(char *line, t_mshell_data *data)
{
	char	*expanded;

	expanded = expand_tokens(line, data);
	return (expanded);
}

/* Heredoc open: open, create or reset temp file for heredoc 
	- O_TRUNC = empty file if it already exists
	- 0600 give permissions to read and write
*/
static int	open_heredoc_file(void)
{
	int	fd;
	
	fd = open(".heredoc_temp", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		return (perror("heredoc"), -1);
	return (fd);
}

/* Heredoc loop: reads user input */
static void	heredoc_loop(int fd, t_tokens *token, t_mshell_data *data)
{
	char	*line;
	
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_printf("minishell: warning: heredoc delimited ");
			ft_printf("by end-of-file (wanted `%s')\n", token->redir_file);
			break ;
		}
		if (!ft_strcmp(line, token->redir_file))
		{
			free(line);
			break ;
		}
		write_line(fd, line, token, data);
		free(line);
	}
}


/* Handle heredoc: create temp fd, use readline to read user input, expand and write in file. Then close fd */
int	handle_heredoc(t_tokens *token, t_mshell_data *data)
{
	int	fd;
	
	fd = open_heredoc_file(); // opening temp file to write
	if (fd < 0)
		return (perror("heredoc"), -1);
	heredoc_loop(fd, token, data); //reads lines until delimeter
	close(fd);	//closes write fd
	token->heredoc_fd = open(".heredoc_temp", O_RDONLY); //reopen to read
	if (token->heredoc_fd < 0)
		return (perror("heredoc"), -1);
	return (token->heredoc_fd);
}


