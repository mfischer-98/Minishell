/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 11:59:42 by mefische          #+#    #+#             */
/*   Updated: 2025/10/03 11:59:42 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main (void)
{
	char	*prompt;

	while (1)
	{
		prompt = readline("\x1b[32mmili_fisc@Mel\x1b[0m:~$> ");
		if (!prompt)
			exit(0);
		//check_command(prompt);
		if (!check_exit(prompt))
			exit(0); //change signal later and add error message
		ft_printf("%s: command not found\n", prompt);
		add_history(prompt);
	}
}
