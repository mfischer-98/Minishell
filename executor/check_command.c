#include "../minishell.h"

static char *find_command_in_path(char *cmd, t_env *env_list)
{
	char	**paths;
	char	*path_env;
	char	*full_path;
	int		i;

	if (access(cmd, X_OK) == 0)
		return (cmd);
	path_env = NULL;
	while (env_list)
	{
		if (ft_strncmp(env_list->var, "PATH=", 5) == 0)
		{
			path_env = env_list->var + 5;
			break ;
		}
		env_list = env_list->next;
	}
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(full_path, cmd);
		
		if (access(full_path, X_OK) == 0)
		{
			free_array(paths, i + 1);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	
	free_array(paths, i);
	return (NULL);
}
static void execute_external_command(char **commandline, t_env *env_list)
{
	char	*cmd_path;
	char	**envp;
	int		i;

	cmd_path = find_command_in_path(commandline[0], env_list);
	if (!cmd_path)
	{
		ft_printf("minishell: command not found: %s\n", commandline[0]);
		exit(127);
	}
	i = 0;
	while (env_list && env_list->next) // convert env to array cause exec only takes array
	{
		i++;
		env_list = env_list->next;
	}
	envp = malloc(sizeof(char *) * (i + 2));
	if (!envp)
		return ;
	i = 0;
	env_list = env_list ? env_list : env_list; // reset?
	while (env_list && env_list->var)
	{
		envp[i] = env_list->var;
		i++;
		env_list = env_list->next;
	}
	envp[i] = NULL;
	execve(cmd_path, commandline, envp);
	perror("execve");
	exit(1);
}

void run_command(char **commandline, t_mshell_data *data)
{
	pid_t	pid;
	int		status;

	if (!commandline || !commandline[0])
		return ;
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
	else // Not a built-in - fork to then execute
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return ;
		}
		if (pid == 0) //child
		{
			execute_external_command(commandline, data->env_var);
		}
		else //parent
		{
			waitpid(pid, &status, 0);
		}
	}
}

void executor(t_mshell_data *data)
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
