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
	reset_signals();
}


/* Handle heredoc: fork a child to read heredoc input so Ctrl+C only interrupts heredoc, not the shell */
int handle_heredoc(t_tokens *token, t_mshell_data *data)
{
	int fd;
	pid_t pid;
	int status;

	fd = open_heredoc_file(); // Open temp file for heredoc
	if (fd < 0)
		return (perror("heredoc"), -1);
	pid = fork();
	if (pid < 0)
		return (close(fd), perror("fork"), -1);
	if (pid == 0) // Child process: set heredoc signals and run heredoc loop
	{
		set_heredoc_signals();
		heredoc_loop(fd, token, data);
		close(fd);
		exit(0); // Exit child after heredoc
	}
	close(fd); // Parent process: wait for child to finish heredoc
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)// If child was interrupted by SIGINT (Ctrl+C), propagate status and abort heredoc
		return (data->exit_status = 128 + WTERMSIG(status), write(1, "\n", 1), -1);
	token->heredoc_fd = open(".heredoc_temp", O_RDONLY);// Reopen heredoc file for reading in parent
	if (token->heredoc_fd < 0)
		return (perror("heredoc"), -1);
	return (token->heredoc_fd);
}


