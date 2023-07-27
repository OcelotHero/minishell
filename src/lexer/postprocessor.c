/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postprocessor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 02:00:42 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/22 19:17:07 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

char	*unquote(char *wrd, int n, int *unquoted)
{
	int		i;
	int		count;
	char	*data;
	char	*res;

	i = -1;
	count = 0;
	while (wrd[++i] && i < n)
		if (wrd[i] != '\'' && wrd[i] != '"' && (wrd[i] != '\\' || ++i))
			count++;
	data = malloc(sizeof(*data) * (count + 1));
	if (!data)
		return (NULL);
	i = -1;
	*unquoted = 0;
	res = data;
	while (wrd[++i] && i < n)
	{
		*unquoted |= (wrd[i] == '\'' || wrd[i] == '"');
		if (wrd[i] != '\'' && wrd[i] != '"' && (wrd[i] != '\\' || ++i))
			*(data++) = wrd[i];
	}
	*data = '\0';
	return (res);
}

int	append_token(t_list **tokens, t_list *node, char *data, int unquoted)
{
	((t_token *)node->content)->data = ((t_token *)(*tokens)->content)->data;
	((t_token *)node->content)->type = ((t_token *)(*tokens)->content)->type;
	if (((t_token *)node->content)->type & CMD)
		((t_token *)node->content)->type = ARGS;
	else if (((t_token *)node->content)->type & (ARGS | OPTS1 | OPTS2))
	{
		((t_token *)(*tokens)->content)->type = ARGS;
		if (data[0] == '-' && (ft_isalnum(data[1])
				|| (data[1] == '-' && ft_isalpha(data[2]))))
		((t_token *)(*tokens)->content)->type = OPTS1
				+ (data[1] == '-') * (OPTS2 - OPTS1);
	}
	((t_token *)(*tokens)->content)->data = data;
	((t_token *)(*tokens)->content)->type |= unquoted * QUOT;
	((t_token *)(*tokens)->content)->type |= is_builtin(data) * BUILTIN;
	node->next = (*tokens)->next;
	(*tokens)->next = node;
	*tokens = node;
	return (0);
}

int	split_token(t_list **tokens, char *wrd, int n)
{
	int		unquoted;
	char	*data;
	t_token	*token;
	t_list	*node;

	data = unquote(wrd, n, &unquoted);
	if (!data)
		return (1);
	if (ft_strlen(wrd) == n)
	{
		free(((t_token *)(*tokens)->content)->data);
		((t_token *)(*tokens)->content)->data = data;
		((t_token *)(*tokens)->content)->type |= unquoted * QUOT;
		return (0);
	}
	token = malloc(sizeof(*token));
	node = ft_lstnew(token);
	if (!token || !node)
	{
		free(data);
		if (token)
			free(token);
		return (1);
	}
	return (append_token(tokens, node, data, unquoted));
}

int	postprocess(t_list **tokens, int *n, char *wrd)
{
	int		type;
	t_token	*token;

	type = token_type(&wrd[n[0]]);
	if (type & (SPACES | END))
	{
		if ((n[1] != n[0] || (n[2] > 0 && n[2] == n[0]))
			&& split_token(tokens, &wrd[n[1]], n[0] - n[1]))
			return (1);
		if (n[2] > 0 && n[2] == n[0])
		{
			token = (*tokens)->content;
			token->type |= is_builtin(token->data) * BUILTIN;
		}
		n[1] = n[0] + 1;
	}
	return (0);
}

int	postprocess_tokens(t_list *tokens)
{
	int		*n;
	char	*s;

	while (tokens)
	{
		s = ((t_token *)tokens->content)->data;
		n = (int []){-1, 0, ft_strlen(s), DEFAULT};
		while (++(n[0]) <= n[2])
		{
			if (n[3] != SQUOTE && s[n[0]] == '\\')
				(n[0])++;
			else if ((s[n[0]] == '\'' && n[3] == SQUOTE)
				|| (s[n[0]] == '"' && n[3] == DQUOTE))
				n[3] = DEFAULT;
			else if ((s[n[0]] == '\'' || s[n[0]] == '"') && n[3] == DEFAULT)
				n[3] = (s[n[0]] != '"') * SQUOTE + (s[n[0]] == '"') * DQUOTE;
			else if (n[3] == DEFAULT && postprocess(&tokens, n, s))
				return (1);
		}
		tokens = tokens->next;
	}
	return (0);
}
