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

int	builtin_pwd(t_list **token_list, t_list **var_list)
{
	char	buf[BUFSIZ];
	char	*dir;

	(void) var_list;
	*token_list = (*token_list)->next;
	dir = getcwd(buf, BUFSIZ);
	if (dir)
		printf("%s\n", dir);
	else
		perror("pwd");
	return (!dir);
}
