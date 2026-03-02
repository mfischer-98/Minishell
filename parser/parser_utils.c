/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 14:47:45 by mefische          #+#    #+#             */
/*   Updated: 2026/03/02 15:09:10 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_unclosed_quotes(t_tokens *tokens)
{
	int		sq;
	int		dq;

	sq = 0;
	dq = 0;
	while (tokens)
	{
		for (int i = 0; tokens->input[i]; i++)
		{
			if (tokens->input[i] == '\"')
				dq++;
			else if (tokens->input[i] == '\'')
				sq++;
		}
		tokens = tokens->next;
	}
	return ((dq % 2 != 0) || (sq % 2 != 0));
}

/*Update underscore variable
	- holds last argument of previous command
	- call executor
	- after executor keep last command */
void	update_underscore(t_mshell_data *data)
{
	t_env		*vars;
	t_tokens	*tokens;
	char		*last_command;

	tokens = data->tokens;
	while (tokens->next)
		tokens = tokens->next;
	last_command = ft_strdup(tokens->input);
	vars = data->env_var;
	while (vars)
	{
		if (!ft_strncmp(vars->var, "_=", 2))
			vars->var = ft_strjoin("_=", last_command);
		vars = vars->next;
	}
}

/* Shell level security < 1000 
	- if >= 1000: reset to 1 and no error */
void	check_shell_level(t_env *env)
{
	int		level;
	t_env	*temp;

	temp = env;
	while (temp)
	{
		if (!ft_strncmp(temp->var, "SHLLVL", 6))
			level = ft_atoi(temp->var + 6); //skip SHLLVL=
		temp = temp->next;
	}
	if (level >= 1000)
	{
		temp->var = ft_strdup("SHLLVL=1");
	}
}