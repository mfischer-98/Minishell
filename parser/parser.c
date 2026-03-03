/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:27:39 by mefische          #+#    #+#             */
/*   Updated: 2026/03/03 09:58:49 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Syntax errors: unclosed quotes, pipe at the end of commandline or start alone, malformed redirs
	- Error message + exit 2 */
void	parser(t_mshell_data *data)
{
	if (!data || !data->tokens)
		return ;
	if (check_unclosed_quotes(data->tokens))
	{
		ft_putstr_fd("minishell: Error: Unclosed quotes\n", 2);
		data->exit_status = 2;
		return ;
	}
	check_shell_level(data->env_var);
	if (check_pipe_syntax(data->tokens))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2); //make more specific
		data->exit_status = 2;
		return ;
	}
	if (check_redir_syntax(data->tokens))
	{
		data->exit_status = 2;
		return ;
	}
	executor(data);
	update_underscore(data);
}

/* Pipe syntax errors: 
	- pipe at start or end
	- consecutive pipes | | or ||
	- redirection followed by pipe */
int	check_pipe_syntax(t_tokens *tokens)
{
	t_tokens	*temp;

	temp = tokens;
	if (temp && temp->type == NODE_PIPE)
		return (1);
	while (temp && temp->next)
	{
		if (temp->type == NODE_PIPE && temp->next->type == NODE_PIPE)
			return (1);
		if ((temp->type == NODE_OUT || temp->type == NODE_IN
				|| temp->type == NODE_APPEND) && (temp->next->type == NODE_PIPE))
			return (1);
		temp = temp->next;
	}
	if (temp && temp->type == NODE_PIPE)
		return (1);
	return (0);
}

static void redir_error(t_node_type type)
{
	if (type == NODE_IN)
		ft_putstr_fd("minishell: syntax error near unexpected token `<'\n", 2);
	else if (type == NODE_OUT)
		ft_putstr_fd("minishell: syntax error near unexpected token `>'\n", 2);
	else if (type == NODE_APPEND)
		ft_putstr_fd("minishell: syntax error near unexpected token `>>'\n", 2);
	else if (type == NODE_HERE)
		ft_putstr_fd("minishell: syntax error near unexpected token `<<'\n", 2);
	return ;
}

/* Redirect syntax errors:
	- consecutive redirects
	- missing file name
	- invalid name token: commands or pipe
*/
int	check_redir_syntax(t_tokens *tokens)
{
	t_tokens	*temp;

	temp = tokens;
	while (temp && temp->next)
	{
		if (temp->type == NODE_OUT || temp->type == NODE_IN || temp->type == NODE_APPEND)
		{
			if (temp->next->type == NODE_OUT || temp->next->type == NODE_IN
					|| temp->next->type == NODE_APPEND || temp->next->type == NODE_HERE)
				return (redir_error(temp->next->type), 1);
			if (!temp->next || temp->next->type == NODE_PIPE)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
				return (1);
			}
		}
		if ((temp->next->type == NODE_OUT || temp->next->type == NODE_IN
				|| temp->next->type == NODE_APPEND) && (temp->type == NODE_PIPE))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}
