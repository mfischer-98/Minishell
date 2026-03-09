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
	int	len;

	len = ft_strlen(name);
	if (((name[0] == '\'' || name[0] == '"')
			&& name[0] == name[len - 1]))
	{
		name[len - 1] = '\0';
		return (ft_strdup(name + 1));
	}
	return (ft_strdup(name));
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
