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

int	builtin_pwd(char **opts, t_list **var_list)
{
	char	buf[BUFSIZ];
	char	*dir;

	(void) opts;
	(void) var_list;
	dir = getcwd(buf, BUFSIZ);
	if (!dir)
		return (error_msg(errno, E_PWDV, strerror(errno)));
	ft_dprintf(1, "%s\n", dir);
	return (0);
}
