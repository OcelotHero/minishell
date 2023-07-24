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

static int	comp_token(const t_token *t1, const t_token *t2)
{
	return (!(t1->type & (CMD | OPTS1 | OPTS2 | ARGS)) || (t1->type & QUOT)
		|| ft_strlen(t1->data));
}

static void	free_token(void *token)
{
	free(((t_token *)token)->data);
	free(token);
}

void	trim_token(t_list **tokens)
{
	ft_lstremove_if(tokens, NULL, comp_token, free_token);
}
