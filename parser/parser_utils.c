/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 14:47:45 by mefische          #+#    #+#             */
/*   Updated: 2026/03/05 09:43:15 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_unclosed_quotes(t_tokens *tokens)
{
	int		sq;
	int		dq;
	int		i;

	sq = 0;
	dq = 0;
	while (tokens)
	{
		i = 0;
		while (tokens->input[i])
		{
			if (tokens->input[i] == '\"')
				dq++;
			else if (tokens->input[i] == '\'')
				sq++;
			i++;
		}
		tokens = tokens->next;
	}
	if (sq % 2 != 0)
		return (1);
	if (dq % 2 != 0)
		return (1);
	return (0);
}

/*Update underscore variable
	- holds last argument of previous command
	- call executor
	- after executor keep last command */
void	update_underscore(t_mshell_data *data)
{
	t_env		*vars;
	t_tokens	*temp;
	char		*last_command;

	if (!data->tokens)
		return ;
	temp = data->tokens;
	while (temp->next)
		temp = temp->next;
	last_command = ft_strdup(temp->input);
	vars = data->env_var;
	while (vars)
	{
		if (!ft_strncmp(vars->var, "_=", 2))
		{
			free(vars->var);
			vars->var = ft_strjoin("_=", last_command);
			break ;
		}
		vars = vars->next;
	}
	free(last_command);
}

/* Shell level security < 1000 
	- if >= 1000: reset to 1 and no error 
	- ft_atoi(temp->var + 6 = skip SHLVL= */
void	check_shell_level(t_env *env)
{
	int		level;
	t_env	*temp;

	temp = env;
	while (temp)
	{
		if (!ft_strncmp(temp->var, "SHLVL", 5))
		{
			level = ft_atoi(temp->var + 6);
			if (level >= 1000)
			{
				free (temp->var);
				temp->var = ft_strdup("SHLVL=1");
			}
			return ;
		}
		temp = temp->next;
	}
}
