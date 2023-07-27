/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 15:58:08 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/19 15:58:08 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

char	*var_value(char *key, t_list *var_list)
{
	int		i;
	t_list	*node;

	node = var_list;
	while (node)
	{
		i = 0;
		while (((char *)node->content)[i] == *(key + i))
			i++;
		if (((char *)node->content)[i] == '=' &&
			!ft_isalnum(*(key + i)) && *(key + i) != '_')
			return (&((char *)node->content)[i + 1]);
		node = node->next;
	}
	return (NULL);
}

int	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "unset"));
}
