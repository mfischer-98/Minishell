/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 11:08:33 by mefische          #+#    #+#             */
/*   Updated: 2026/01/27 14:46:46 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int check_exit(char *prompt)
{
	char	**split;
	int		exit_code;
	int		i;

	if (!prompt || !*prompt)
		return (1);
	split = ft_split(prompt, ' ');
	if (!split || !split[0] || ft_strcmp(split[0], "exit") != 0)
		return (1);
		//return (free_split(split), 1);
	exit_code = 0;
	if (split[1])
	{
		i = 0;
		while (split[1][i] && ft_isdigit(split[1][i]))
			i++;
		if (split[1][i] != '\0')
			return (ft_printf("exit: invalid argument\n"), 1);
			//return (ft_printf("exit: invalid argument\n"), free_split(split), 1);
		exit_code = ft_atoi(split[1]);
	}
	//free_split(split);
	exit(exit_code);  // Sai já aqui
}