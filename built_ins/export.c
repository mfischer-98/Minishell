#include "../minishell.h"

static char *trim_left(char *str)
{
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}

int	handle_no_equal(char *arg, t_mshell_data *data)
{
	if (arg[0] == '\0')
		return (0);
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

	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
		return (1);
	name = ft_substr(arg, 0, equal_sign - arg);
	if (!name)
		return (1);
	if (!identifier_valid(name))
	{
		ft_printf("minishell: export: `%s': is not a valid identifier\n", trim_left(arg));
		free(name);
		return (1);
	}
	if (!check_env_list(name, data))
		add_env_list(arg, data);
	else
		update_env_list(arg, data);
	free(name);
	return (0);
}

int	process_export(char *str, t_mshell_data *data)
{
	char	*new_str;

	if (str[0] == '-')
	{
		ft_printf("minishell: export: `%s': not a valid identifier\n", str);
		return (2);
	}
	new_str = trim_outer_quotes(str);
	if (ft_strnstr(new_str, "+=", ft_strlen(new_str)))
		return (handle_append(new_str, data));
	if (ft_strchr(new_str, '='))
		return (handle_equal(new_str, data));
	return (handle_no_equal(new_str, data));
}

int	export(char **commandline, t_mshell_data *data)
{
	int	status;
	int	final_status;
	int	i;

	if (!data || !commandline || !commandline[0])
		return (1);
	if (!commandline[1])
	{
		print_export(data);
		return (0);
	}
	status = 0;
	final_status = 0;
	i = 1; //start on the word next to export
	while (commandline[i])
	{
		status = process_export(commandline[i], data);
		if (status != 0)
			final_status = status; //not sure if final status is correct
		i++;
	}
	return (final_status);
}
