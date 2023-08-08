/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snagulap <snagulap@student.42heilbronn. 	+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 20:17:25 by snagulap          #+#    #+#             */
/*   Updated: 2023/07/12 20:17:25 by snagulap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * 
 * @param	opts		options -- echo $?, $HOME ...
 * @param	vars		Shell environment variables
 * @return 				returns 0 
 */
int	builtin_echo(char **opts, t_list **vars)
{
	int	i;
	int	j;
	int	flag;

	(void) vars;
	i = 0;
	flag = (!ft_strcmp(opts[0], "echo")) << 2;
	while (opts[++i])
	{
		j = 0;
		if (((flag & 4) || i == 1) && opts[i][j++] == '-' && !(flag & 2))
		{
			while (opts[i][j++] == 'n')
				flag |= (opts[i][j] == '\0' && ((flag & 4) || j == 2));
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
