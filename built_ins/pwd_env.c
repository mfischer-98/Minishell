/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:31:26 by mefische          #+#    #+#             */
/*   Updated: 2025/10/28 12:25:50 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	pwd(void)
{
	char	*path;
	
	path = getcwd(NULL, 0);
	if (!path)
		return(perror("pwd"), 1);
	ft_printf("%s\n", path);
	//free(path);
	return (0);
}

int	env(char **commandline, t_mshell_data *data)
{
	int	i;

	i = 0;
	if (!data->env_var)
	{
		perror("Error in environment variables.");
		return(0);
	}
	if (commandline[1])
	{
		ft_printf("env: '%s': No such file or directory\n", commandline[1]);
		return(127); //0 on success, 127 if arguments provided
	}
	print_env(data);
	//oldpwd, shell level e _="" need cd
	return(0);
}
