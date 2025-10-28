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

void	pwd(void)
{
	char	*path;
	
	path = getcwd(NULL, 0);
	printf("%s\n", path);
}

int	env(char **commandline, char **envp)
{
	int	i;

	i = 0;
	if (!envp)
	{
		perror("Error in environment variables.");
		return(1); //check exit status
	}
	if (commandline[1])
	{
		printf("env: '%s': No such file or directory\n", commandline[1]);
		return(1);
	}
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	return(0);
}
