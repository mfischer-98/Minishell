#include "../minishell.h"

void run_command(char **commandline, t_mshell_data *data)
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

void check_command(t_mshell_data *data)
{
	t_tokens	*temp;

	if (!data || !data->tokens)
		return ;
	temp = data->tokens;
	if (temp->type == NODE_WORD)
	{
		run_command(array_join(&data->tokens), data);
	}
	//else if (temp->type == NODE_HERE)
	else
		ft_printf("Error\n");
	// while (temp)
	// {
	// 	if (temp->type == NODE_PIPE)
	// 		check_command(&temp);
	// 	temp = temp->next;
	// } 
}
