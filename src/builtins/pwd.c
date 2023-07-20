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
	(void) opts;
	dir = getcwd(buf, BUFSIZ);
	if (dir)
		printf("%s\n", dir);
	else
		perror("pwd");
	exit(!dir);
}
