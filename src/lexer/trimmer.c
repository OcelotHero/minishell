/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trimmer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 19:17:07 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/22 19:17:07 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	trim_blank_tokens(t_list **tokens)
{
	t_list	*node;
	t_token	*token;

	if (!tokens || !(*tokens))
		return (0);
	node = *tokens;
	token = (t_token *)node->content;
	if ((token->type & SEMI) || ((token->type & (CMD | OPTS1 | OPTS2 | ARGS))
			&& !(token->type & QUOT) && !ft_strlen(token->data)))
	{
		*tokens = node->next;
		free(token->data);
		free(token);
		free(node);
		return (trim_blank_tokens(tokens));
	}
	if (BONUS && (token->type & WILD) && expand_wildcard(node))
		return (1);
	return (trim_blank_tokens(&(node->next)));
}

int	trim_parenthesis_token(t_list **tokens, int ptype)
{
	int		type;
	t_list	*node;

	if (!tokens || !(*tokens))
		return (0);
	node = *tokens;
	type = ((t_token *)node->content)->type;
	if (ptype && ((ptype & RPAREN) || !(type & (OR_IF | AND_IF))))
	{
		if (type & ptype)
		{
			*tokens = node->next;
			free(((t_token *)node->content)->data);
			free(((t_token *)node->content));
			free(node);
			if (type & RPAREN)
				return (0);
			return (trim_parenthesis_token(tokens, RPAREN));
		}
		return (trim_parenthesis_token(&(node->next), ptype));
	}
	if (((t_token *)node->content)->type & (LPAREN | OR))
		trim_parenthesis_token(&(node->next), LPAREN);
	return trim_parenthesis_token(&(node->next), 0);
}

int	trim(t_list **tokens)
{
	return (trim_blank_tokens(tokens) || trim_parenthesis_token(tokens, 0));
}