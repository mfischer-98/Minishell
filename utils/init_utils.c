/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 09:21:41 by mefische          #+#    #+#             */
/*   Updated: 2026/02/13 09:54:21 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	initialize(t_mshell_data **data, char **envp)
{
	*data = malloc(sizeof(t_mshell_data));
	if (!data)
	{
		perror("malloc");
		return ;
	}
	(*data)->tokens = NULL;
	(*data)->env_var = NULL;
	(*data)->expander = malloc(sizeof(t_expander));
	if (!(*data)->expander)
	{
		free(*data);
		return ;
	}
	(*data)->exit_status = 0;
	create_envp(envp, *data);
}

void	init_expander(t_expander **expander)
{
	(*expander)->result = ft_strdup("");
	(*expander)->i = 0;
	(*expander)->in_single = 0;
	(*expander)->in_double = 0;
}
