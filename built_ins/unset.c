#include "../minishell.h"

// Bash variable names:

// Must be non-empty.
// Must start with a letter or underscore.
// Remaining chars must be letters, digits, or underscore.
// Anything else (starting with a digit, -, @, !, etc.) is invalid as an identifier for export / unset
// return 1 if valid, 0 if invalid

void	unset_env(char *str, t_mshell_data *data)
{
	t_env	*temp;

	if (!str || !data)
		return ;
	if (check_env_list(str))
		return ;
	temp = data->env_var;
	while (temp)
	{
		if (!ft_strcmp(str, temp.var))
			//remove node
		temp = temp->next;
	}
}

int	unset(char **commandline, t_mshell_data *data)
{
	int	;

	if (!data)
		return (1)
	if (!commandline[1] || !commandline)
		return (0);
	if (commandline[1][0] == '-' && commandline[1][1] != '\0')
	{
		ft_printf("unset: -%s: invalid option in minishell\n", commandline[1][1])
		return (2);
	}
	i = 1;
	while (commandline[i])
	{
		if (identifier_valid(commandline[i]))
			unset_env(commandline[i], data);
		i++;
	}
	return (0);
}