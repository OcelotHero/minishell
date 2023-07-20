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
	j = 0;
	flag = 0;
	while (opts[++i])
	{
		if (i == 1 && opts[i][j++] == '-')
		{
			while (opts[i][j++] == 'n')
				flag = opts[i][j] == '\0';
			if (flag)
				continue ;
		}
		if (flag & 2)
			printf(" ");
		flag |= 2;
		printf("%s", opts[i]);
	}
	if (!(flag & 1))
		printf("\n");
	exit(0);
}
