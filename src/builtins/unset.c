/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 04:07:08 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/13 04:07:08 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	comp_var(const char *s1, const char *s2)
{
	return (ft_strncmp(s1, s2, ft_strlen(s2) + 1) != '=');
}

int	builtin_unset(t_list **token_list, t_list **var_list)
{
	int	type;

	*token_list = (*token_list)->next;
	type = ((t_token *)(*token_list)->content)->type;
	while (type < SPACES)
	{
		ft_lstremove_if(var_list, ((t_token *)(*token_list)->content)->data,
			comp_var, free);
		*token_list = (*token_list)->next;
		type = ((t_token *)(*token_list)->content)->type;
	}
	return (0);
}
