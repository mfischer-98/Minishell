#include "../minishell.h"

void	create_tokens(char *prompt, t_tokens **tokens)
{
	int			i;
	int			start;
	char		in_quote;
	char		quote_char;

	i = 0;
	start = 0;
	in_quote = 0;
	quote_char = 0;
	while (prompt[i])
	{
		if (!in_quote && (prompt[i] == '"' || prompt[i] == '\''))
		{
			if (i > start)
				add_token(tokens, ft_substr(prompt, start, i - start), NODE_UNKNOWN);
			in_quote = 1;
			quote_char = prompt[i];
			start = i + 1;
			i++;
		}
		else if (in_quote && prompt[i] == quote_char)
		{
			add_token(tokens, ft_substr(prompt, start, i - start), (quote_char == '"') ? NODE_DOUBLE_QUOTE : NODE_SINGLE_QUOTE);
			in_quote = 0;
			i++;
			while (prompt[i] == ' ')
				i++;
			start = i;
		}
		else if (!in_quote && prompt[i] == ' ')
		{
			if (i > start)
				add_token(tokens, ft_substr(prompt, start, i - start), NODE_UNKNOWN);
			while (prompt[i] == ' ')
				i++;
			start = i;
		}
		else
			i++;
	}
	if (i > start)
		add_token(tokens, ft_substr(prompt, start, i - start), in_quote ? ((quote_char == '"') ? NODE_DOUBLE_QUOTE : NODE_SINGLE_QUOTE) : NODE_UNKNOWN);
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
    if (type == NODE_DOUBLE_QUOTE || type == NODE_SINGLE_QUOTE)
    {
        char *trimmed = ft_strtrim(input, " \t\n\v\f\r");
        free(input);
        input = trimmed;
    }
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
		else if (temp->type == NODE_UNKNOWN || temp->type == NODE_SINGLE_QUOTE || temp->type == NODE_DOUBLE_QUOTE) // aspas possivelmente nao palavra
			temp->type = NODE_WORD;
		temp = temp->next;
	}
}
