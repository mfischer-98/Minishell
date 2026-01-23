#include "../minishell.h"

//print env in alphabetical order function, need to add

int	handle_no_equal(char *arg, t_mshell_data *data)
{
	if (!identifier_valid(arg))
	{
		ft_printf("minishell: export: ' %s': is not a valid identifier\n", arg);
		free(arg);
		return (1);
	}
	//return (add_env_list(arg, NULL, data)); //no value
	print_env(data); //tirar
	return (0); //tirar
}

int	handle_equal(char *arg, t_mshell_data *data)
{
	char	*equal_sign;
	char	*name;
	char	*value;
	int		result;

	result = 0;
	equal_sign = ft_strchr(arg, '=');
	name = ft_substr(arg, 0, equal_sign - arg);
	if (!name)
		return (1); //nao tem nada antes do igual
	if (!identifier_valid(name))
	{
		ft_printf("minishell: export: ' %s': is not a valid identifier\n", arg);
		free(name);
		return (1);
	}
	value = remove_quotes(equal_sign++);
/* 	if (!check_env_list(name, data)); //0 = nao está na lista
		//add_env_list(name, value, data);
	else
		//update_env_list(name, value, data); */
	free(name);
	free(value);
	print_env(data); //tirar
	return (result);
}

int	process_export(char *commandline, t_mshell_data *data)
{
	if (commandline[0] == '-')
	{
		//erro
		return (2);
	}
	//append nao entendi
	if (ft_strchr(commandline, '='))
		//return (handle_equal(commandline, data));
	//return (handle_no_equal(commandline, data));
	print_env(data); //tirar
	return (0); //tirar
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
		print_env(data); //this has to be in alphabetical order, still need to make that function
		return (0);
	}
	status = 0;
	i = 0;
	while (commandline[i])
	{
		status = process_export(commandline[i], data);
		if (status != 0)
			final_status = status; //nao sei ainda ou final status
		i++;
	}
	return (final_status);
}