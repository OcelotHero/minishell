/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snagulap <snagulap@student.42heilbronn.	+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 04:07:02 by snagulap          #+#    #+#             */
/*   Updated: 2023/07/13 04:07:02 by snagulap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * 
 * @param	arg			options -- Creates new env variable..
 * @param	vars		Shell environment variables
 * @param	n			index of args
 * @return 				returns 0 on success, 1 on failure 
 */
static int	export_new(t_list **vars, char **arg, int n)
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
		(*arg)[n] = '+';
	}
	else if (!(*arg)[n])
		var = ft_strjoin(*arg, "=");
	else
		var = ft_strdup(*arg);
	if (!var)
	{
		free(node);
		return (1);
	}
	node->content = var;
	ft_lstadd_front(vars, node);
	return (0);
}

/**
 * 
 * @param	arg			options -- searches for existing env variable and replaces
 * @param	vars		Shell environment variables
 * @return 				returns 0 on success, 1 on failure 
 */
static int	export_var(t_list **vars, char **arg, int n)
{
	int		dif;
	char	*var;
	t_list	*node;

	node = *vars;
	while (node)
	{
		dif = ft_strncmp(node->content, *arg, n + 1);
		if (!dif || (dif == ('=' - '+') && ((char *)node->content)[n] == '+'))
		{
			if (dif)
				var = ft_strjoin(node->content, &(*arg)[n + 2]);
			else
				var = ft_strdup(*arg);
			if (!var)
				return (1);
			free(node->content);
			node->content = var;
			return (0);
		}
		node = node->next;
	}
	return (export_new(vars, arg, n));
}

/**
 * 
 * @param	vars		vars -- prints env variable..
 * @return 				returns 0 on success
 */
static int	print_env(t_list **vars)
{
	int		i;
	int		flag;
	char	*var;
	t_list	*node;

	node = *vars;
	while (node)
	{
		i = -1;
		flag = 0;
		var = node->content;
		write(1, "declare -x ", 11);
		while (var[++i])
		{
			if (var[i] == '"' || var[i] == '\\' || var[i] == '$')
				write(1, "\\", 1);
			write(1, &var[i], 1);
			if (!flag && var[i] == '=')
				write(1, "\"", 1);
			flag |= var[i] == '=';
		}
		write(1, "\"\n", 2);
		node = node->next;
	}
	return (0);
}

/**
 * 
 * @param	opts		options -- export VARIABLE = "VALUE"..
 * @param	vars		Shell environment variables
 * @return 				returns 0 on success, 1 on failure 
 */
int	builtin_export(char **opts, t_list **vars)
{
	int		i;
	int		j;
	int		error;

	i = 0;
	error = 0;
	while (opts[++i])
	{
		j = 0;
		error = 0;
		if ((ft_isalpha(opts[i][j]) || opts[i][j] == '_') && ++j)
			while (opts[i][j] && opts[i][j] != '='
				&& (ft_isalnum(opts[i][j]) || opts[i][j] == '_'))
				j++;
		if ((opts[i][j] != '=' && ft_strncmp(&opts[i][j], "+=", 2)
			&& opts[i][j]) || !j)
			error |= error_msg(1, E_EXPO, opts[i]);
		if (!error && export_var(vars, &opts[i], j))
			return (error_msg(errno, E_MLOC, strerror(errno)));
	}
	return (error || (i == 1 && print_env(vars)));
}
