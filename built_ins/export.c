#include "../minishell.h"

static char *trim_left(char *str)
{
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}

int	handle_no_equal(char *arg, t_mshell_data *data)
{
	if (!identifier_valid(arg))
	{
		ft_printf("minishell: export: `%s': is not a valid identifier\n", trim_left(arg));
		return (1);
	}
	return (add_env_list(arg, data));
}

int	handle_equal(char *arg, t_mshell_data *data)
{
	char	*equal_sign;
	char	*name;
	int		result;

	result = 0;
	equal_sign = ft_strchr(arg, '=');
	name = ft_substr(arg, 0, equal_sign - arg);
	if (!name)
		return (1); //nao tem nada antes do igual
	if (!identifier_valid(name))
	{
		ft_printf("minishell: export: `%s': is not a valid identifier\n", trim_left(arg));
		free(name);
		return (1);
	}
	if (!check_env_list(name, data)) //0 = nao está na lista
		add_env_list(arg, data);
	else
		update_env_list(arg, data);
	return (result);
}

int	process_export(char *commandline, t_mshell_data *data)
{
	if (commandline[0] == '-')
	{
		ft_printf("minishell: export: `%s': not a valid identifier\n", commandline);
		//or does not support options
		return (2);
	}
	//append missing
	if (ft_strchr(commandline, '='))
		return (handle_equal(commandline, data));
	return (handle_no_equal(commandline, data));
}

int	export(char **commandline, t_mshell_data *data)
{
	int	status;
	int	final_status;
	int	i;

	if (!data || !commandline)
		return (1);
	if (!commandline[1] || !*commandline[1])
	{
		print_export(data); //this has to be in alphabetical order, still need to make that function
		return (0);
	}
	status = 0;
	final_status = 0;
	i = 1; //start on the word next to export
	while (commandline[i])
	{
		status = process_export(commandline[i], data);
		if (status != 0)
			final_status = status; //nao sei ainda ou final status
		i++;
	}
	return (final_status);
}