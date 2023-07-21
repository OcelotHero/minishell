/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 22:33:58 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/09 22:33:58 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	builtin_env(char **opts, t_list **var_list)
{
	t_list	*node;

	if (opts[1] != NULL)
	{
		printf("env: `%s': No such file or directory\n", opts[1]);
		exit(1);
	}
	node = *var_list;
	while (node)
	{
		printf("%s\n", (char *)node->content);
		node = node->next;
	}
	exit(0);
}