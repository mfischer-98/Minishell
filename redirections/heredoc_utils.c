/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:19:30 by mefische          #+#    #+#             */
/*   Updated: 2026/02/17 17:19:30 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	write_line(int fd, char *line, t_tokens *token, t_mshell_data *data)
{
	char	*expanded;
	
	if (!token->quote_delim) //delimeter was not quoted
	{
		expanded = expand_heredoc_line(line, data);
		write(fd, expanded, ft_strlen(expanded));
		free(expanded);
	}
	else //quoted delimeter = do not expand variables
		write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

int	process_delimeter_quotes(t_tokens *token)
{
	int		len;
	char	*new_delim; //without quotes

	if (!token->redir_file || !token->redir_file[0])
		return (0);
	len = ft_strlen(token->redir_file);
	if (len < 2)
		return (0);
	if ((token->redir_file[0] == '\'' && token->redir_file[len - 1] == '\'')
		|| (token->redir_file[0] == '"' && token->redir_file[len - 1] == '"'))
	{
		new_delim = ft_substr(token->redir_file, 1, len - 2);
		if (!new_delim)
			return (0);
		free(token->redir_file);
		token->redir_file = new_delim;
		return (1);
	}
	return (0);
}
