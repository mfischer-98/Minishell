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
		echo(commandline, data->tokens);
	else if (!ft_strcmp(commandline[0], "export"))
		export(commandline, data);
	else if (!ft_strcmp(commandline[0], "unset"))
	 	unset(commandline, data);
	/* else
		execve(ft_strjoin(getcwd(NULL, 0), "./minishell"), commandline, envp); */
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
	while (temp)
	{
		if (temp->type == NODE_WORD || temp->type == NODE_SINGLE_QUOTE)
		{
			temp->type = NODE_WORD; //precisa para nao ter erro de unclosed quotes, weird
			expanded = expand_tokens(temp->input, data);
			free(temp->input);
			temp->input = expanded;
		}
		temp = temp->next;
	}
	commands = array_join(&data->tokens);
	if (data->tokens && data->tokens->type == NODE_WORD && commands && commands[0])
		run_builtin(commands, data);
	//else if (temp->type == NODE_HERE)
	else
		ft_printf("Error\n");
}
