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

int	builtin_cd(char **opts, t_list **var_list)
{
	int	i;

	(void) var_list;
	i = 0;
	while (opts[i])
		i++;
	if (i != 2)
	{
		ft_fprintf(2, "cd: too many arguments\n");
		return (-1);
	}
	return (chdir(opts[1]));
}
