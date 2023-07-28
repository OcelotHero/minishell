/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 04:06:59 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/13 04:06:59 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	builtin_exit(char **opts, t_list **var_list)
{
	int	i;

	if (opts[2])
	{
		ft_fprintf(2, "exit: too many arguments\n");
		return (-1);
	}
	if (!opts[1])
		return (0);
	i = -1;
	while (opts[1][++i])
	{
		if (!i && (opts[1][i] != '-' || opts[1][i] != '+'
				|| !ft_isdigit(opts[1][i])))
			break ;
		else if (!ft_isdigit(opts[1][i]))
			break ;
	}
	if (opts[1][i])
	{
		ft_fprintf(2, "exit: %s: numeric argument required\n", opts[1]);
		return (-1);
	}
	return (ft_atoi(opts[1]));
}
