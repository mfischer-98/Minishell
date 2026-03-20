/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 00:00:00 by mefische          #+#    #+#             */
/*   Updated: 2026/03/20 12:30:34 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_child(t_fork_data *fork,	t_tokens *segment)
{
	int	exit_code;

	close(fork->saved);
	if (fork->pipefd[1] != -1)
		dup2(fork->pipefd[1], STDOUT_FILENO);
	if (fork->pipefd[0] >= 0)
		close(fork->pipefd[0]);
	if (fork->pipefd[1] >= 0)
		close(fork->pipefd[1]);
	fork->data->exit_status = apply_redirects(segment, fork->data);
	exit_code = fork->data->exit_status;
	if (exit_code != 0)
	{
		free_data(fork->data);
		exit(exit_code);
	}
	if (is_builtin(fork->cmd))
	{
		run_command(fork->cmd, fork->data);
		free_data(fork->data);
		exit(exit_code);
	}
	else
		execute_external_command(fork->cmd, fork->data, segment);
}

static int	handle_parent(int pipefd[], int saved, t_mshell_data *data,
	pid_t pid)
{
	int	status;

	if (pipefd[0] != -1)
		dup2(pipefd[0], STDIN_FILENO);
	else
	{
		dup2(saved, STDIN_FILENO);
		waitpid(pid, &status, 0);
		close(saved);
		if (WIFEXITED(status))
			data->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			data->exit_status = 128 + WTERMSIG(status);
		return (0);
	}
	if (pipefd[1] >= 0)
		close(pipefd[1]);
	return (1);
}

static int	handle_fork(t_fork_data *fork_data)
{
	pid_t	pid;
	int		result;

	pid = fork();
	if (pid == 0)
		handle_child(fork_data, *fork_data->tokens);
	else if (pid > 0)
	{
		result = handle_parent(fork_data->pipefd, fork_data->saved,
				fork_data->data, pid);
		if (fork_data->next)
		{
			close(fork_data->pipefd[0]);
			*fork_data->tokens = fork_data->next->next;
		}
		return (free_array(fork_data->cmd, array_size(fork_data->cmd)), result);
	}
	free_array(fork_data->cmd, array_size(fork_data->cmd));
	return ((dup2(fork_data->saved, STDIN_FILENO),
			close(fork_data->saved), 0));
}

static int	process_segment(t_mshell_data *data, t_tokens **tokens, int saved)
{
	t_tokens	*next;
	char		**cmd;
	int			pipefd[2];
	t_fork_data	fork_data;

	next = *tokens;
	pipefd[0] = -1;
	pipefd[1] = -1;
	while (next && next->type != NODE_PIPE)
		next = next->next;
	cmd = build_command(tokens);
	if (!cmd || (next && pipe(pipefd) == -1))
	{
		dup2(saved, STDIN_FILENO);
		if (saved >= 0)
			close(saved);
		return (0);
	}
	fork_data.data = data;
	fork_data.tokens = tokens;
	fork_data.saved = saved;
	fork_data.cmd = cmd;
	fork_data.pipefd = pipefd;
	fork_data.next = next;
	return (handle_fork(&fork_data));
}

void	execute_piped_commands(t_mshell_data *data, t_tokens *tokens)
{
	int	saved;

	saved = dup(STDIN_FILENO);
	if (saved == -1)
		return ;
	while (tokens && process_segment(data, &tokens, saved))
		;
	close(saved);
}
