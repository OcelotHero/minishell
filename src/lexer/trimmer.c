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

int	trim_tokens(t_list **tokens)
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
		return (trim_tokens(tokens));
	}
	if (BONUS && (token->type & WILD) && expand_wildcard(node))
		return (1);
	return (trim_tokens(&(node->next)));
}
