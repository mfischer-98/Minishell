/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:21:09 by mefische          #+#    #+#             */
/*   Updated: 2026/02/13 12:57:24 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Bash variable names:
 Must be non-empty.
 Must start with a letter or underscore.
 Remaining chars must be letters, digits, or underscore.
 Anything else (starting with a digit, -, @, !, etc.) is invalid as an identifier
 return 1 if valid, 0 if invalid
*/

void	unset_env(char *str, t_mshell_data *data)
{
	t_env	*temp;
	t_env	*prev;
	int		len;

	if (!str || !data || !data->env_var)
		return ;
	prev = NULL;
	temp = data->env_var;
	len = ft_strlen(str);
	while (temp)
	{
		if (!ft_strcmp(str, temp->var)
			|| (!ft_strncmp(str, temp->var, len) && temp->var[len] == '='))
		{
			if (!prev)
				data->env_var = temp->next;
			else
				prev->next = temp->next;
			free(temp->var);
			free(temp);
			return ;
		}
		prev = temp;
		temp = temp->next;
	}
}

int	unset(char **commandline, t_mshell_data *data)
{
	int	i;

	if (!data)
		return (1);
	if (!commandline[1] || !commandline)
		return (0);
	if (commandline[1][0] == '-' && commandline[1][1] != '\0')
	{
		ft_printf("unset: -%c: invalid option in minishell\n", commandline[1][1]);
		return (2);
	}
	i = 1;
	while (commandline[i])
	{
		if (identifier_valid(commandline[i]))
			unset_env(commandline[i], data);
		i++;
	}
	return (0);
}
