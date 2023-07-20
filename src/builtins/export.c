/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 04:07:02 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/13 04:07:02 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	export_var(t_list **var_list, t_token *token, int n)
{
	t_list	*node;

	node = *var_list;
	while (node)
	{
		if (!ft_strncmp(node->content, token->data, n + 1))
		{
			node->content = token->data;
			token->data = NULL;
			break ;
		}
		node = node->next;
	}
	if (token->data)
	{
		node = ft_lstnew(token->data);
		if (!node)
			return (1);
		ft_lstadd_front(var_list, node);
		token->data = NULL;
	}
	return (0);
}

int	builtin_export(t_list **token_list, t_list **var_list)
{
	int		type;
	char	*loc;

	*token_list = (*token_list)->next;
	type = ((t_token *)(*token_list)->content)->type;
	while (type < SPACES)
	{
		loc = ft_strchr(((t_token *)(*token_list)->content)->data, '=');
		if (loc)
		{
			if (export_var(var_list, (*token_list)->content,
				loc - ((t_token *)(*token_list)->content)->data))
				return (1);
		}
		*token_list = (*token_list)->next;
		type = ((t_token *)(*token_list)->content)->type;
	}
	return (0);
}
