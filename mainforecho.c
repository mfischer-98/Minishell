#include "minishell.h"

static int	quotes_closed(char *str)
{
	int		single;
	int		dquote;
	int		i;

	single = 0;
	dquote = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !dquote)
			single = !single;
		else if (str[i] == '"' && !single)
			dquote = !dquote;
		i++;
	}
	return (!single && !dquote);
}

static char	*read_full(void)
{
	char	*line;
	char	*full_input;
	char	*temp;

	full_input = NULL;
	while (1)
	{
		if (full_input)
			line = readline("> ");
		else
			line = readline("\x1b[32mminishell\x1b[0m> ");
		if (!line)
		{
			if (full_input)
				free(full_input);
			return (NULL);
		}
		if (full_input)
		{
			temp = ft_strjoin(full_input, "\n");
			free(full_input);
			full_input = ft_strjoin(temp, line);
			free(temp);
			free(line);
		}
		else
			full_input = ft_strdup(line);
		if (has_all_quotes_closed(full_input))
			break ;
		if (full_input == NULL)
			free(line);
	}
	return (full_input);
}

int main (int argc, char **argv, char **envp)
{
	t_mshell_data	*data;
	char			*prompt;
	
	(void)argc;
	(void)argv;
	print_banner();
	initialize(&data, envp);
	while (1)
	{
		prompt = read_full_input();
		if (!prompt)
			exit(0);
		if (!check_exit(prompt))
			exit(0);
		create_tokens(prompt, &data->tokens);
		check_command(data);
		add_history(prompt);
		free(prompt);
		free_list(data->tokens);
		data->tokens = NULL;
	}
}