#include "../minishell.h"

void	initialize(t_mshell_data **data, char **envp)
{
	*data = malloc(sizeof(t_mshell_data));
	if (!data)
	{
		perror("malloc");
		return ;
	}
	(*data)->tokens = NULL;
	(*data)->env_var = NULL;
	create_envp(envp, *data);
}
