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

int	token_type(int i, char *str)
{
	if (((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' '))
		return (SPACES);
	if (str[i] == '(')
		return (LPAREN);
	if (str[i] == ')')
		return (RPAREN);
	if (str[i] == ';')
		return (SEMI);
	if (str[i] == '<')
		return (LESS + (str[i + 1] == '<') * (DLESS - LESS));
	if (str[i] == '>')
		return (GREAT + (str[i + 1] == '>') * (DGREAT - GREAT));
	if (str[i] == '|')
		return (OR + (str[i + 1] == '|') * (OR_IF - OR));
	if (str[i] == '&')
		return (AND + (str[i + 1] == '&') * (AND_IF - AND));
	if (str[i] == '\0')
		return (END);
	return (WORD);
}

int	save_token(t_list **tokens, int start, int n, char *str)
{
	int		i;
	t_list	*node;
	t_token	*token;
	char	*data;

	data = malloc(sizeof(*data) * (n + 1));
	if (!data)
		return (1);
	i = -1;
	while (++i < n)
		data[i] = str[start + i];
	data[i] = '\0';
	token = malloc(sizeof(*token));
	if (!token)
		return (1);
	*token = (t_token){token_type(start, str), data};
	node = ft_lstnew(token);
	if (!node)
		return (1);
	ft_lstadd_back(tokens, node);
	return (0);
}

int	tokenize_line(t_list **tokens, int *i, int *start, char *str)
{
	int	type;
	int	error;

	error = 0;
	type = token_type(*i, str);
	if (type >= SPACES && (!*i || str[*i - 1] != '\\'))
	{
		if ((*i && token_type(*i - 1, str) < SPACES)
			|| (*i && (*i - 1) && str[*i - 2] == '\\'))
			error |= save_token(tokens, *start, *i - *start, str);
		if (type > SPACES)
			error |= save_token(tokens, *i, 1 + (type >= DLESS), str);
		*i += (type >= DLESS);
		*start = *i + 1;
	}
	return (error);
}

int	tokenize(t_list **tokens, char *str)
{
	int	*n;
	int	state;

	n = (int []){-1, 0, ft_strlen(str)};
	state = DEFAULT;
	while (++(n[0]) <= n[2])
	{
		if (state != SQUOTE && str[n[0]] == '\\')
			(n[0])++;
		else if ((str[n[0]] == '\'' && state == SQUOTE)
			|| (str[n[0]] == '"' && state == DQUOTE))
			state = DEFAULT;
		else if ((str[n[0]] == '\'' || str[n[0]] == '"') && state == DEFAULT)
			state = (str[n[0]] != '"') * SQUOTE + (str[n[0]] == '"') * DQUOTE;
		if (state == DEFAULT && tokenize_line(tokens, &n[0], &n[1], str))
			return (1);
	}
	if (state != DEFAULT)
		printf("unexpected EOF while looking for matching %c\n",
			(state == SQUOTE) * '\'' + (state != SQUOTE) * '"');
	else if (((t_token *)ft_lstlast(*tokens)->content)->type != END)
		printf("unexpected EOF with escaped character\n");
	return (state != DEFAULT
		|| ((t_token *)ft_lstlast(*tokens)->content)->type != END);
}
