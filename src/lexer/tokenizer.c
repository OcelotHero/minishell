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

int	data_length(char *wrd, int n)
{
	int	i;
	int	count;
	int	*s;

	i = -1;
	count = 0;
	s = (int []){DEFAULT, DEFAULT};
	while (wrd[++i] && i < n)
	{
		if (((wrd[i + 1] == '"' || wrd[i + 1] == '\\' || wrd[i + 1] == '$')
				&& s[0] == DQUOTE || s[0] == DEFAULT) && wrd[i] == '\\')
			i++;
		else if ((wrd[i] == '\'' && s[0] == SQUOTE)
			|| (wrd[i] == '"' && s[0] == DQUOTE))
			s[0] = DEFAULT;
		else if ((wrd[i] == '\'' || wrd[i] == '"') && s[0] == DEFAULT)
			s[0] = (wrd[i] != '"') * SQUOTE + (wrd[i] == '"') * DQUOTE;
		if (s[0] == s[1])
			count++;
		s[1] = s[0];
	}
	return (count);
}

int	populate_data(char *wrd, int n, char *data)
{
	int	i;
	int	wild;
	int	*s;

	i = -1;
	wild = 0;
	s = (int []){DEFAULT, DEFAULT};
	while (wrd[++i] && i < n)
	{
		if (((wrd[i + 1] == '"' || wrd[i + 1] == '\\' || wrd[i + 1] == '$')
				&& s[0] == DQUOTE || s[0] == DEFAULT) && wrd[i] == '\\')
			i++;
		else if ((wrd[i] == '\'' && s[0] == SQUOTE)
			|| (wrd[i] == '"' && s[0] == DQUOTE))
			s[0] = DEFAULT;
		else if ((wrd[i] == '\'' || wrd[i] == '"') && s[0] == DEFAULT)
			s[0] = (wrd[i] != '"') * SQUOTE + (wrd[i] == '"') * DQUOTE;
		if (s[0] == s[1])
			*(data++) = wrd[i];
		if (s[0] != SQUOTE && wrd[i] == '*' && (!i || wrd[i - 1] != '\\'))
			wild = 1;
		s[1] = s[0];
	}
	*data = '\0';
	return (wild);
}

t_token	*refine_token(char *str, int n, char *data, int type)
{
	t_token	*token;

	token = malloc(sizeof(*token));
	if (!token)
	{
		free(data);
		return (NULL);
	}
	token->type = token_type(str);
	if (populate_data(str, n, data))
		token->type = WILD * BONUS;
	token->data = data;
	if (token->type == WORD)
	{
		if (type & (LESS | GREAT | DLESS | DGREAT))
			token->type = FILES;
		else
			token->type = CMD * (type >= SPACES) + ARGS * (type < SPACES);
		if (data[0] == '-' && (ft_isalnum(data[1])
				|| (data[1] == '-' && ft_isalpha(data[2]))))
			token->type = OPTS1 + (data[1] == '-') * (OPTS2 - OPTS1);
	}
	return (token);
}

int	save_token(t_list **tokens, char *str, int n)
{
	static int	type = SPACES;
	char		*data;
	t_list		*node;
	t_token		*token;

	data = malloc(sizeof(*data) * (data_length(str, n) + 1));
	if (!data)
		return (1);
	token = refine_token(str, n, data, type);
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

int	tokenize_line(t_list **tokens, int *n, char *str)
{
	int	type;
	int	error;

	error = 0;
	type = token_type(&str[n[0]]);
	if (type >= SPACES && (!n[0] || str[n[0] - 1] != '\\'))
	{
		if ((n[0] && token_type(&str[n[0] - 1]) < SPACES)
			|| (n[0] && (n[0] - 1) && str[n[0] - 2] == '\\'))
			error = save_token(tokens, &str[n[1]], n[0] - n[1]);
		if (!error && type > SPACES)
			error = save_token(tokens, &str[n[0]], 1 + (type >= DLESS));
		n[0] += (type >= DLESS);
		n[1] = n[0] + 1;
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
		if (state == DEFAULT && tokenize_line(tokens, n, str))
			return (1);
	}
	return (0);
}
