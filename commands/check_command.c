#include "../minishell.h"

void run_command(char **commandline, t_mshell_data *data)
{
	if (!ft_strcmp(commandline[0], "pwd"))
		pwd();
	// else if (!ft_strcmp(commandline[0], "cd"))
	// 	cd();
	else if (!ft_strcmp(commandline[0], "env"))
		env(commandline, data);
	else if (!ft_strcmp(commandline[0], "echo"))
		ft_echo(commandline);
	else if (!ft_strcmp(commandline[0], "export"))
		export(commandline[1], data);
	// else if (!ft_strcmp(commandline[0], "unset"))
	// 	unset();
	/* else
		execve(ft_strjoin(getcwd(NULL, 0), "./minishell"), commandline, envp); */
}

void check_command(t_mshell_data *data)
{
	t_tokens	*temp;

	temp = data->tokens;
	if (temp->type == NODE_WORD)
	{
		check_vars(temp->input, &data->local_var);
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
