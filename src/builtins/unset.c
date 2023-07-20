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

int	builtin_unset(char **opts, t_list **var_list)
{
	while (*(++opts))
		ft_lstremove_if(var_list, *opts, comp_var, free);
	exit(0);
}
