/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 03:40:27 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/13 03:40:27 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	builtin_cd(t_list **token_list, t_list **var_list)
{
	(void) var_list;
	*token_list = (*token_list)->next;
	return (chdir(((t_token *)(*token_list)->content)->data));
}
