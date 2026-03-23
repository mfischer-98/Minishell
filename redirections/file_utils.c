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

/* Strips quotes from a filename and removes ./ prefix if present
	- Copies result into a new string, skipping all ' and " chars
	- Needed so > "file.txt" and > file.txt behave identically */
char	*strip_file_quotes(char *name)
{
	char	*result;
	int		i;
	int		j;

	if (!ft_strncmp(name, "./", 2))
		name += 2;
	result = ft_strdup(name);
	i = 0;
	j = 0;
	while (result[i])
	{
		if (result[i] != '"' && result[i] != '\'')
			result[j++] = result[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

/* Prints bash-style error for failed file open using errno
	- Handles ENOENT (no such file), EISDIR (is a directory),
	- EACCES (permission denied). Sets exit_status to 1 and returns 1 */
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
	return (1);
}

/* Checks if delimeter has quotes, if yes do not expand */
int	here_quotes(char *str)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(str);
	if (str[i] == '\'' && str[len - 1] == '\'')
		return (1);
	if (str[i] == '\"' && str[len - 1] == '\"')
		return (1);
	return (0);
}

/* Heredoc open: open, create or reset temp file for heredoc 
	- O_TRUNC = empty file if it already exists
	- 0600 give permissions to read and write
*/
int	open_heredoc_file(void)
{
	int	fd;

	fd = open(".heredoc_temp", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		return (perror("heredoc"), -1);
	return (fd);
}
