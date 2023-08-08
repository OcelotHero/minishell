/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snagulap <snagulap@student.42heilbronn.	+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 21:33:49 by snagulap          #+#    #+#             */
/*   Updated: 2023/07/09 21:33:49 by snagulap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * 
 * @param	opts		options -- PWD Gets current working directory..
 * @param	vars		Shell environment variables
 * @return 				returns 0 on success
 */
int	builtin_pwd(char **opts, t_list **vars)
{
	char	buf[BUFSIZ];
	char	*dir;

	(void) opts;
	(void) vars;
	dir = getcwd(buf, BUFSIZ);
	if (!dir)
		return (error_msg(errno, E_PWDV, strerror(errno)));
	ft_dprintf(1, "%s\n", dir);
	return (0);
}
