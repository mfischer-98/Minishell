#include "../minishell.h"

void	create_tokens(char *prompt, t_tokens **tokens)
{
	int			i;
	int			start;

	i = 0;
	start = 0;
	while (prompt[i])
	{
		if (prompt[i] == '"' || prompt[i] == '\'')
		{
			i = handle_quotes(tokens, prompt, prompt[i], i + 1);
			while (prompt[i] == ' ')
				i++;
			start = i;
		}
		else if (prompt[i] == ' ' && i > start)
		{
			add_token(tokens, ft_substr(prompt, start, i - start), NODE_UNKNOWN);
			while (prompt[i] == ' ')
				i++;
			start = i;
		}
		else //skipar outros caracteres
			i++;
	}
	if (i > start) //só criamos token se tiver espaço entre eles
		add_token(tokens, ft_substr(prompt, start, i - start), NODE_UNKNOWN);
	add_type(tokens);
	}

int	handle_quotes(t_tokens **tokens, char *str, char quote, int start)
{
	int	i;

	i = start;
	while (str[i] && str[i] != quote)
		i++;
	if (quote == '"')
		add_token(tokens, ft_substr(str, start, i - start), NODE_DOUBLE_QUOTE);
	else
		add_token(tokens, ft_substr(str, start, i - start), NODE_SINGLE_QUOTE);
	return (i + 1);
}

void	add_token(t_tokens **tokens, char *input, t_node_type type)
{
    t_tokens *new_node;
    t_tokens *node;

    new_node = malloc(sizeof(t_tokens));
    if (!new_node)
		return;
    new_node->input = input;
    new_node->type = type;
    new_node->next = NULL;
    if (*tokens == NULL)
    {
        *tokens = new_node;
        return;
    }
    node = *tokens;
    while (node->next)
        node = node->next;
    node->next = new_node;
}

void	add_type(t_tokens **tokens)
{
	t_tokens	*temp;

	temp = *tokens;
	//se o token tiver ", só acaba quando achar outras "
	while (temp != NULL)
	{
		if (!ft_strcmp(temp->input, "|"))
			temp->type = NODE_PIPE;
		else if (!ft_strcmp(temp->input, ">>"))
			temp->type = NODE_APPEND;
		else if (!ft_strcmp(temp->input, "<<"))
			temp->type = NODE_HERE;
		else if (!ft_strcmp(temp->input, ">"))
			temp->type = NODE_OUT;
		else if (!ft_strcmp(temp->input, "<"))
			temp->type = NODE_IN;
		else if (temp->type == NODE_UNKNOWN)
			temp->type = NODE_WORD;
		temp = temp->next;
	}
}
