/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 02:45:01 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/13 02:45:01 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	insert_matches(t_list *tokens, glob_t *gs, int n, int type)
{
	t_list	*node;
	t_token	*token;

	if (n == gs->gl_pathc)
		return (n);
	if (!n)
	{
		free(((t_token *)tokens->content)->data);
		((t_token *)tokens->content)->data = gs->gl_pathv[n];
		((t_token *)tokens->content)->type = (type & ~WILD);
		return (insert_matches(tokens, gs, n + 1, (type & ~WILD)));
	}
	token = malloc(sizeof(*token));
	node = ft_lstnew(token);
	if (!token || !node)
	{
		if (token)
			free(token);
		return (n);
	}
	token->data = gs->gl_pathv[n];
	token->type = ((type & CMD) == CMD) * ARGS + !(type & CMD) * type;
	node->next = tokens->next;
	tokens->next = node;
	return (insert_matches(node, gs, n + 1, type));
}

int	expand_wildcard(t_list *token)
{
	int		n;
	char	*data;
	glob_t	gs;

	n = glob(((t_token *)token->content)->data, GLOB_TILDE, NULL, &gs);
	if (n && n != GLOB_NOMATCH)
		return (error_msg(errno, E_MLOC, strerror(errno)));
	if (n == GLOB_NOMATCH)
	{
		data = ((t_token *)token->content)->data;
		data = unquote(data, ft_strlen(data), &n, 0);
		if (!data)
			return (error_msg(errno, E_MLOC, strerror(errno)));
		free(((t_token *)token->content)->data);
		((t_token *)token->content)->data = data;
		((t_token *)token->content)->type ^= WILD;
		return (0);
	}
	n = insert_matches(token, &gs, 0, ((t_token *)token->content)->type);
	while (*(gs.gl_pathv + n))
		free(*(gs.gl_pathv++ + n));
	free(gs.gl_pathv);
	if (n < gs.gl_pathc)
		return (error_msg(errno, E_MLOC, strerror(errno)));
	return (0);
}
