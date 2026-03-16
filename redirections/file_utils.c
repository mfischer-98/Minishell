/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 17:06:58 by mefische          #+#    #+#             */
/*   Updated: 2026/02/25 17:06:58 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*strip_file_quotes(char *name)
{
	char	*result;
		
	if (!ft_strncmp(name, "./", 2))
		name += 2;
	result = ft_strdup(name);
	int i = 0;
	int j = 0;
	while (result[i])
	{
		if (result[i] != '"' && result[i] != '\'')
			result[j++] = result[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

int	check_fd_error(char *name, t_mshell_data *data)
{
	ft_putstr_fd("minishell: ", 2);
	if (name && name[0] != '\0')
	{
		ft_putstr_fd(name, 2);
		ft_putstr_fd(": ", 2);
	}
	if (errno == ENOENT)
		ft_putstr_fd("No such file or directory\n", 2);
	else if (errno == EISDIR)
		ft_putstr_fd("Is a directory\n", 2);
	else if (errno == EACCES)
		ft_putstr_fd("Permission denied\n", 2);
	data->exit_status = 1;
	exit(1);
}
