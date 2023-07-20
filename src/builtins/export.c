/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 04:07:02 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/13 04:07:02 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	export_var(t_list **var_list, char **arg, int n)
{
	t_list	*node;

	node = *var_list;
	while (node)
	{
		if (!ft_strncmp(node->content, *arg, n + 1))
		{
			free(node->content);
			node->content = *arg;
			*arg = NULL;
			return (0);
		}
		node = node->next;
	}
	node = ft_lstnew(*arg);
	if (!node)
		return (1);
	ft_lstadd_front(var_list, node);
	*arg = NULL;
	return (0);
}



int	builtin_export(char **opts, t_list **var_list)
{
	int		i;
	char	*loc;

	while (*(++opts))
	{
		i = 0;
		if (ft_isalpha((*opts)[i]) || (*opts)[i] == '_')
		{
			i++;
			while ((*opts)[i] && (*opts)[i] != '=')
			{
				if (!ft_isalnum((*opts)[i]) && (*opts)[i] != '_')
					break ;
				i++;
			}
		}
		if ((*opts)[i] != '=')
		{
			printf("export: `%s': not a valid identifier", *opts);
			exit(1);
		}
		if (export_var(var_list, opts, i))
			exit(1);
	}
	exit(0);
}
