/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 22:33:58 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/09 22:33:58 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	builtin_env(t_list **token_list, t_list **var_list)
{
	t_list	*node;

	*token_list = (*token_list)->next;
	node = *var_list;
	while (node)
	{
		printf("%s\n", (char *)node->content);
		node = node->next;
	}
	return (0);
}
