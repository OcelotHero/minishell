/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 02:56:37 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/07 02:56:37 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	token_type(char *str)
{
	if ((*str == '\t' || *str == '\n' || *str == ' '))
		return (SPACES);
	if (*str == '(')
		return (LPAREN);
	if (*str == ')')
		return (RPAREN);
	if (*str == ';')
		return (SEMI);
	if (*str == '<')
		return (LESS + (*(str + 1) == '<') * (DLESS - LESS));
	if (*str == '>')
		return (GREAT + (*(str + 1) == '>') * (DGREAT - GREAT));
	if (*str == '|')
		return (OR + (*(str + 1) == '|') * (OR_IF - OR));
	if (*str == '&')
		return (AND + (*(str + 1) == '&') * (AND_IF - AND));
	if (*str == '\0')
		return (END);
	return (WORD);
}

t_token	*refine_token(char *str, int *n, char *data, t_list *vars)
{
	t_token	*token;

	token = malloc(sizeof(*token));
	if (!token)
	{
		free(data);
		return (NULL);
	}
	token->type = populate_data(str, n, data, vars);
	if ((token->type & WILD) && !BONUS)
		token->type ^= WILD;
	if (token->type == WORD)
		token->type = token_type(str);
	token->data = data;
	if (token->type == WORD)
	{
		if (n[1] & (LESS | GREAT | DLESS | DGREAT))
			token->type |= FILES;
		else if (data[0] == '-' && (ft_isalnum(data[1])
				|| (data[1] == '-' && ft_isalpha(data[2]))))
			token->type |= OPTS1 + (data[1] == '-') * (OPTS2 - OPTS1);
		else
			token->type |= CMD * (n[1] >= SPACES) + ARGS * (n[1] < SPACES);
	}
	return (token);
}

int	save_token(t_list **tokens, char *str, int n, t_list *vars)
{
	static int	type = SPACES;
	char		*data;
	t_list		*node;
	t_token		*token;

	data = malloc(sizeof(*data)
		* (data_length(str, (int []){n, type}, vars) + 1));
	if (!data)
		return (1);
	token = refine_token(str, (int []){n, type}, data, vars);
	if (!token)
		return (1);
	node = ft_lstnew(token);
	if (!node)
	{
		free(data);
		free(token);
		return (1);
	}
	ft_lstadd_back(tokens, node);
	type = token_type(str);
	return (0);
}

int	tokenize_line(t_list **tokens, int *n, char *str, t_list *vars)
{
	int	type;
	int	error;

	error = 0;
	type = token_type(&str[n[0]]);
	if (type >= SPACES)
	{
		if (type >= SPACES && n[1] != n[0])
			error = save_token(tokens, &str[n[1]], n[0] - n[1], vars);
		if (!error && type > SPACES)
			error = save_token(tokens, &str[n[0]], 1 + (type >= DLESS), vars);
		n[0] += (type >= DLESS);
		n[1] = n[0] + 1;
	}
	return (error);
}

int	tokenize(t_list **tokens, char *str, t_list *vars)
{
	int	*n;
	int	state;

	n = (int []){-1, 0, ft_strlen(str)};
	state = DEFAULT;
	while (++(n[0]) <= n[2])
	{
		if (state != SQUOTE && str[n[0]] == '\\' && !str[n[0] + 1])
			break ;
		if (state != SQUOTE && str[n[0]] == '\\')
			(n[0])++;
		else if ((str[n[0]] == '\'' && state == SQUOTE)
			|| (str[n[0]] == '"' && state == DQUOTE))
			state = DEFAULT;
		else if ((str[n[0]] == '\'' || str[n[0]] == '"') && state == DEFAULT)
			state = (str[n[0]] != '"') * SQUOTE + (str[n[0]] == '"') * DQUOTE;
		else if (state == DEFAULT && tokenize_line(tokens, n, str, vars))
			return (1);
	}
	if (state != DEFAULT)
		printf("unexpected EOF while looking for matching `%c'\n",
			(state == SQUOTE) * '\'' + (state != SQUOTE) * '"');
	else if (n[0] <= n[2])
		printf("unexpected EOF with escaped character\n");
	return (state != DEFAULT || n[0] == n[2]);
}
