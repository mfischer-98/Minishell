#include "../minishell.h"

void run_builtin(char **commandline, t_mshell_data *data)
{
	if (!ft_strcmp(commandline[0], "pwd"))
		pwd();
	/* else if (!ft_strcmp(commandline[0], "cd"))
	 	cd(data, commandline); */
	else if (!ft_strcmp(commandline[0], "env"))
		env(commandline, data);
	else if (!ft_strcmp(commandline[0], "echo"))
		echo(commandline);
	else if (!ft_strcmp(commandline[0], "export"))
		export(commandline, data);
	else if (!ft_strcmp(commandline[0], "unset"))
	 	unset(commandline, data);
	/* else
		execve(ft_strjoin(getcwd(NULL, 0), "./minishell"), commandline, envp); */
}

static int	check_unclosed_quotes(t_tokens *tokens)
{
	int			i;
	int			sq;
	int			dq;
	t_tokens	*temp;

	temp = tokens;
	while (temp)
	{
		sq = 0;
		dq = 0;
		i = 0;
		while (temp->input[i])
		{
			if (temp->input[i] == '\"')
				dq++;
			if (temp->input[i] == '\'')
				sq++;
			i++;
		}
		temp = temp->next;
	}
	if ((dq % 2 != 0) || (dq % 2 != 0))
		return (1);
	return (0);
}

//Check command function is our parser/executor
void check_command(t_mshell_data *data)
{
	t_tokens	*temp;
	char		*expanded;
	char		**commands;

	if (!data || !data->tokens)
		return ;
	temp = data->tokens;
	if (check_unclosed_quotes(data->tokens))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("Error: Unclosed quotes\n", 2);
		data->exit_status = 1;
		return ;
	}
	while (temp)
	{
		expanded = expand_tokens(temp->input, data);
		free(temp->input);
		temp->input = expanded;
		temp = temp->next;
	}
	commands = array_join(&data->tokens);
	if (data->tokens && data->tokens->type == NODE_WORD && commands && commands[0])
		run_builtin(commands, data);
}
