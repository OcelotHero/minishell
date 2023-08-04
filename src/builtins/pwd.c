/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 21:33:49 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/09 21:33:49 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	builtin_pwd(char **opts, t_list **var_list)
{
	char	buf[BUFSIZ];
	char	*dir;

	(void) var_list;
	if (opts[1])
		return (error_msg(1, E_PWDA));
	dir = getcwd(buf, BUFSIZ);
	if (!dir)
		return (error_msg(errno, E_PWDV, strerror(errno)));
	ft_dprintf(1, "%s\n", dir);
	return (0);
}
