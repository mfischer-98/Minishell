/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:31:26 by mefische          #+#    #+#             */
/*   Updated: 2025/10/14 15:49:38 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pwd()
{
	char	*path;
	
	path = getcwd(NULL, 0);
	printf("%s\n", path);
}

void	env()
{
	printf("%s\n", getenv("PATH"));
}