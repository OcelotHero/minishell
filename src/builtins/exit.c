/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snagulap <snagulap@student.42heilbronn.	+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 04:06:59 by snagulap          #+#    #+#             */
/*   Updated: 2023/07/13 04:06:59 by snagulap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * 
 * @param	opts		options -- exit
 * @param	vars	Shell environment variables
 * @return 				returns error number if fails, otherwise returns 0 
 */
int	builtin_exit(char **opts, t_list **vars)
{
	int	i;

	(void )vars;
	if (!opts[1])
		return (g_errno);
	i = -1;
	while (opts[1][++i])
	{
		if (!i && (opts[1][i] == '-' || opts[1][i] == '+'))
			continue ;
		if (!ft_isdigit(opts[1][i]))
			break ;
	}
	if (opts[1][i] || !i)
		return (error_msg(!LINUX * 255 + LINUX * 2, E_EXTN, opts[1]) && 0);
	if (opts[2])
		return (error_msg(1, E_EXTA));
	g_errno = ft_atoi(opts[1]) & 0xff;
	return (0);
}
