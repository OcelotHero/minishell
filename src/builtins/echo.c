/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 20:17:25 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/12 20:17:25 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	builtin_echo(char **opts, t_list **var_list)
{
	int	i;
	int	j;
	int	flag;

	(void) var_list;
	i = 0;
	flag = 0;
	while (opts[++i])
	{
		j = 0;
		if (opts[i][j++] == '-' && !(flag & 2))
		{
			while (opts[i][j++] == 'n')
				flag |= opts[i][j] == '\0';
			if ((flag & 1) && !opts[i][j - 1])
				continue ;
		}
		if (flag & 2)
			ft_dprintf(1, " ");
		flag |= 2;
		ft_dprintf(1, "%s", opts[i]);
	}
	if (!(flag & 1))
		ft_dprintf(1, "\n");
	return (0);
}
