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
			return (ft_printf("Tem exit\n"), 0);
	return (1);
}