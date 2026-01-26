#include "../minishell.h"

void	free_array(char **array, int n)
{
	int	i;

	i = 0;
	while(i < n)
	{
		free(array[i]);
		i++;
	}
	free(array);
}