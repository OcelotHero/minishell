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

char	*unquote(char *wrd, int n, int *quot, int glob)
{
	int		i;
	int		count;
	char	*data[2];

	i = -1;
	count = 0;
	while (wrd[++i] && i < n)
		if (wrd[i] != '\'' && wrd[i] != '"'
			&& (wrd[i] != '\\' || (glob && wrd[i + 1] == '*') || ++i))
			count++;
	data[0] = malloc(sizeof(*data[0]) * (count + 1));
	if (!data[0])
		return (NULL);
	i = -1;
	*quot = 0;
	data[1] = data[0];
	while (wrd[++i] && i < n)
	{
		*quot |= (wrd[i] == '\'' || wrd[i] == '"');
		if (wrd[i] != '\'' && wrd[i] != '"'
			&& (wrd[i] != '\\' || (glob && wrd[i + 1] == '*') || ++i))
			*(data[0]++) = wrd[i];
	}
	*(data[0]) = '\0';
	return (data[1]);
}

int	append_token(t_list **tokens, t_list *node, char *data, int quot)
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
	((t_token *)(*tokens)->content)->type |= quot * QUOT;
	((t_token *)(*tokens)->content)->type |= is_builtin(data) * BUILTIN;
	node->next = (*tokens)->next;
	(*tokens)->next = node;
	*tokens = node;
	return (0);
}

int	split_token(t_list **tokens, char *wrd, int n)
{
	int		quot;
	char	*data;
	t_token	*token;
	t_list	*node;

	data = unquote(wrd, n, &quot, ((t_token *)(*tokens)->content)->type & WILD);
	if (!data)
		return (1);
	if (ft_strlen(wrd) == n)
	{
		free(((t_token *)(*tokens)->content)->data);
		((t_token *)(*tokens)->content)->data = data;
		((t_token *)(*tokens)->content)->type |= quot * QUOT;
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
	return (append_token(tokens, node, data, quot));
}

int	postprocess_token(t_list **tokens, int *n, char *wrd)
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

int	postprocess(t_list *tokens)
{
	int		*n;
	char	*s;

	while (tokens)
	{
		if (((t_token *)tokens->content)->type < SPACES)
		{
			s = ((t_token *)tokens->content)->data;
			n = (int []){-1, 0, ft_strlen(s), DEFAULT};
			while (++(*n) <= n[2])
			{
				if (n[3] != SQUOTE && s[*n] == '\\')
					(*n)++;
				else if ((s[*n] == '\'' && n[3] == SQUOTE)
					|| (s[*n] == '"' && n[3] == DQUOTE))
					n[3] = DEFAULT;
				else if ((s[*n] == '\'' || s[*n] == '"') && n[3] == DEFAULT)
					n[3] = (s[*n] != '"') * SQUOTE + (s[*n] == '"') * DQUOTE;
				else if (n[3] == DEFAULT && postprocess_token(&tokens, n, s))
					return (error_msg(errno, E_MLOC, strerror(errno)));
			}
		}
		tokens = tokens->next;
	}
	return (0);
}
