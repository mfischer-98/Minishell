/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mefische <mefische@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 14:29:52 by mefische          #+#    #+#             */
/*   Updated: 2026/02/13 14:29:52 by mefische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//checks if we have closed brackets
static int	valid_bracket(char	*str)
{
	int	i;
	int	open;
	int	close;

	open = 0;
	close = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '{')
			open++;
		else if (str[i] == '}')
			close++;
		i++;
	}
	if (open != close && open >= 1)
		return (0);
	return (1);
}

/*finds brackets and expands what is inside followed by text*/
char	*handle_brackets(char	*str, int len, int start, t_mshell_data *data)
{
	char	*var_name;
	int		i;

	if (!valid_bracket(str))
		return (NULL);
	i = start + 1;
	while (i < len && str[i] && str[i] != '}')
		i++;
	if (i >= len || str[i] != '}')
		return (NULL);
	len = i - (start + 1);
	var_name = ft_substr(str, start + 1, len);
	if (!var_name)
		return (NULL);
	data->expander->i = i + 1;
	return (var_name);
}

void	var_name_error(char *input, t_mshell_data *data)
{
	ft_printf("minishell: unclosed the brackets\n");
	data->expander->i = ft_strlen(input);
	data->exit_status = 127;
}

void	append_result(char	*str, t_mshell_data *data)
{
	char	*temp;
	char	*old;

	if (str == NULL)
		temp = ft_strdup("");
	else
		temp = ft_strdup(str);
	old = data->expander->result;
	data->expander->result = ft_strjoin(data->expander->result, temp);
	free(old);
	free(temp);
}
