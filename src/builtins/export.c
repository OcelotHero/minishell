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

static int export_new(t_list **var_list, char **arg, int n)
{
	char	*var;
	t_list	*node;

	node = ft_lstnew(NULL);
	if (!node)
		return (-1);
	if ((*arg)[n] == '+')
	{
		(*arg)[n] = '\0';
		var = ft_strjoin(*arg, &(*arg)[n + 1]);
		if (!var)
			return (-1);
		(*arg)[n] = '+';
	}
	else
	{
		var = *arg;
		*arg = NULL;
	}
	node->content = var;
	ft_lstadd_front(var_list, node);
	return (0);
}

static int	export_var(t_list **var_list, char **arg, int n)
{
	int		dif;
	char	*var;
	t_list	*node;

	node = *var_list;
	while (node)
	{
		dif = ft_strncmp(node->content, *arg, n + 1);
		if (!dif || dif == ('=' - '+'))
		{
			if (dif)
				var = ft_strjoin(node->content, &(*arg)[n + 2]);
			if (dif && !var)
				return (-1);
			if (!dif)
				var = *arg;
			if (!dif)
				*arg = NULL;
			free(node->content);
			node->content = var;
			return (0);
		}
		node = node->next;
	}
	return (export_new(var_list, arg, n));
}

int	builtin_export(char **opts, t_list **var_list)
{
	int		i;
	int		j;
	char	*loc;

	i = 0;
	while (opts[++i])
	{
		j = 0;
		if (ft_isalpha(opts[i][j]) || opts[i][j] == '_')
		{
			j++;
			while (opts[i][j] && ft_isalnum(opts[i][j]) && opts[i][j] != '=')
				j++;
		}
		if (opts[i][j] && opts[i][j] != '=' && ft_strncmp(&opts[i][j], "+=", 2))
		{
			ft_fprintf(2, "export: `%s': not a valid identifier", *opts);
			return (-1);
		}
		if (opts[i][j] && export_var(var_list, opts, j))
			return (-1);
	}
	if (i == 1)
		return (builtin_env(opts, var_list));
	return (0);
}
