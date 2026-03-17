/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:21:13 by mefische          #+#    #+#             */
/*   Updated: 2026/03/16 17:58:46 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_execve(char **commandline, t_mshell_data *data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), data->exit_status = 1, (void)0);
	if (pid == 0)
	{
		sig_default(SIGINT);
		sig_default(SIGQUIT);
		execute_external_command(commandline, data, data->tokens);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		data->exit_status = 128 + WTERMSIG(status);
}

void	run_command(char **commandline, t_mshell_data *data)
{
	if (!commandline || !commandline[0] || commandline[0][0] == '\0')
	{
		data->exit_status = 0;
		return ;
	}
	if (!ft_strcmp(commandline[0], "pwd"))
		data->exit_status = pwd();
	else if (!ft_strcmp(commandline[0], "cd"))
		data->exit_status = cd(data, commandline);
	else if (!ft_strcmp(commandline[0], "env"))
		data->exit_status = env(commandline, data);
	else if (!ft_strcmp(commandline[0], "echo"))
		data->exit_status = echo(commandline);
	else if (!ft_strcmp(commandline[0], "export"))
		data->exit_status = export(commandline, data);
	else if (!ft_strcmp(commandline[0], "unset"))
		data->exit_status = unset(commandline, data);
	else if (!ft_strcmp(commandline[0], "exit"))
		check_exit(commandline, data);
	else
		ft_execve(commandline, data);
}

static int	has_pipes(t_mshell_data *data)
{
	t_tokens	*temp;

	temp = data->tokens;
	while (temp)
	{
		if (temp->type == NODE_PIPE)
			return (1);
		temp = temp->next;
	}
	return (0);
}

static void	skip_empty_tokens(t_tokens **tokens)
{
    t_tokens  *current;
    t_tokens  *prev;
    t_tokens  *next;

    if (!tokens || !*tokens)
        return ;
    current = *tokens;
    prev = NULL;
    while (current)
    {
        next = current->next;
        if (!current->input || current->input[0] == '\0')
        {
            if (prev)
                prev->next = next;
            else
                *tokens = next;
            free(current->input);
            free(current);
        }
        else
            prev = current;
        current = next;
    }
}

static int	first_empty(t_mshell_data *data, char *str)
{
	if (str[0] == '\"' && str[1] == '\"')
	{
		data->exit_status = 127;
		return (1);
	}
	if (!str)
	{
		data->exit_status = 127;
		return (1);
	}
	return (0);
}

void	executor(t_mshell_data *data)
{
	char	**commands;

	if (first_empty(data, data->tokens->input))
		return (print_cmd_not_found("\'\'"));
	expand_all_tokens(data);
	skip_empty_tokens(&data->tokens);
	if (!data->tokens)
	{
		data->exit_status = 0;
		return ;
	}
	if (!prep_heredoc(data))
		return ;
	if (has_pipes(data))
		return (execute_piped_commands(data, data->tokens), (void)0);
	commands = array_join(data->tokens);
	if (!ft_strcmp(data->tokens->input, ">") || !ft_strcmp(data->tokens->input,
			">>") || !ft_strcmp(data->tokens->input, "<"))
		return (redirect_start(data->tokens, data));
	if (commands && commands[0] && data->tokens->type == NODE_WORD)
	{
		if (!has_redirect(data->tokens))
			run_command(commands, data);
		else if (is_builtin(commands))
			run_builtin_redirects(commands, data);
		else
			ft_execve(commands, data);
	}
}
