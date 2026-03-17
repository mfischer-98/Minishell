/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 00:00:00 by mefische          #+#    #+#             */
/*   Updated: 2026/03/17 10:45:14 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void handle_child(int has_next, int pipefd[], char **cmd, t_mshell_data *data, t_tokens *segment)
{
    if (has_next)
    {
        dup2(pipefd[1], STDOUT_FILENO);
    }
    close(pipefd[0]);
    close(pipefd[1]);

    data->exit_status = apply_redirects(segment, data);
    if (data->exit_status != 0)
        exit(data->exit_status);
    if (is_builtin(cmd))
    {
        run_command(cmd, data);
        exit(data->exit_status);
    }
    else
        execute_external_command(cmd, data, segment);
}

static int handle_parent(int has_next, int pipefd[], int saved, t_mshell_data *data, pid_t pid)
{
    int status;

    if (has_next)
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
    close(pipefd[1]);
    return (1);
}

static int fail_saved(int saved)
{
    dup2(saved, STDIN_FILENO);
    close(saved);
    return (0);
}

static int	process_segment(t_mshell_data *data, t_tokens **tokens, int saved)
{
	t_tokens	*next = *tokens;
	char		**cmd;
	int			pipefd[2] = {-1, -1};
	pid_t		pid;

	while (next && next->type != NODE_PIPE)
		next = next->next;
	if (!(cmd = build_command(tokens)) || (next && pipe(pipefd) == -1))
		return (fail_saved(saved));
	if ((pid = fork()) == 0)
		handle_child(next != NULL, pipefd, cmd, data, *tokens);
	else if (pid > 0)
	{
		int result = handle_parent(next != NULL, pipefd, saved, data, pid);
		if (next)
		{
			close(pipefd[0]);
			*tokens = next->next;
		}
		return (free(cmd), result);
	}
	return (free(cmd), fail_saved(saved));
}

void	execute_piped_commands(t_mshell_data *data, t_tokens *tokens)
{
    int saved = dup(STDIN_FILENO);
    if (saved == -1)
        return;
    while (tokens && process_segment(data, &tokens, saved));
}
