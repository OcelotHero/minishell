/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snagulap <snagulap@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 22:33:58 by snagulap          #+#    #+#             */
/*   Updated: 2023/07/09 22:33:58 by snagulap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * 
 * @param	opts		options -- env prints the environment list..
 * @param	vars		Shell environment variables
 * @return 				returns 0
 */
int	builtin_env(char **opts, t_list **vars)
{
	t_list	*node;

	if (opts[1] && !ft_strcmp(opts[1], "what"))
		return (0);
	if (opts[1])
		return (error_msg(2, E_ENVA, opts[1]));
	node = *vars;
	while (node)
	{
		ft_dprintf(1, "%s\n", (char *)node->content);
		node = node->next;
	}
	return (0);
}
