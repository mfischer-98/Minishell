/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 11:08:33 by mefische          #+#    #+#             */
/*   Updated: 2025/10/07 14:41:16 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int check_exit (char *prompt)
{
	char *trimmed;
	int	i;

	i = 0;
	trimmed = ft_strtrim(prompt, " ");
	while (trimmed[i])
		i++;
	trimmed[i] = '\0';
	if (ft_strncmp(trimmed, "exit", 4) == 0)
		if (trimmed[4] == ' ' || trimmed[4] == '\0')
			return (ft_printf("Tem exit\n"), free(trimmed), 0);
	free(trimmed);
	return (1);
}