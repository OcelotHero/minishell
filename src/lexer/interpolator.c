/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpolator.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 17:25:01 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/11 17:25:01 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static int	n_digit(int num)
{
	int	n;

	n = 0;
	while (num >= 10)
	{
		num /= 10;
		n++;
	}
	return (n + 1);
}

void	interpolation_error(int **n, char **data)
{
	int	error;

	error = g_errno;
	*data += n_digit(error);
	while (error >= 10)
	{
		*(--(*data)) = (error % 10) + '0';
		error /= 10;
	}
	*(--(*data)) = error + '0';
	++(*n[0]);
	*data += n_digit(g_errno);
}

int	interpolation_length(int **n, char *str, t_list *vars)
{
	int			count;
	char		*val;

	if (str[*n[0]] == '~')
		return (ft_strlen(var_value("HOME", vars)));
	if ((str[*n[0] + 1] == '"' || str[*n[0] + 1] == '\'') && *n[1] == DEFAULT)
		return (0);
	if (!ft_isalnum(str[*n[0] + 1]) && str[*n[0] + 1] != '?'
		&& str[*n[0] + 1] != '_' && str[*n[0] + 1] != '*')
		return (1);
	if (ft_isdigit(str[*n[0] + 1]) || str[*n[0] + 1] == '*'
		|| str[*n[0] + 1] == '?')
		return ((str[++(*n[0])] == '?') * n_digit(g_errno));
	val = var_value(&str[*n[0] + 1], vars);
	while (ft_isalnum(str[*n[0] + 1]) || str[*n[0] + 1] == '_')
		(*n[0])++;
	if (!val)
		return (0);
	count = 0;
	while (*val)
	{
		count += 1 + (*val == '\'' || *val == '"' || *val == '\\');
		val++;
	}
	return (count);
}

int	interpolation_value(int **n, char *str, t_list *vars, char **data)
{
	int		type;
	char	*val;

	val = var_value("HOME", vars);
	if (str[*n[0]] != '~')
		val = var_value(&str[*n[0]+ 1], vars);
	while (ft_isalnum(str[*n[0] + 1]) || str[*n[0] + 1] == '_'
		|| str[*n[0]] == '~')
		(*n[0])++;
	if (!val)
		return (0);
	type = WORD;
	while (*val)
	{
		if (*val == '\'' || *val == '"')
			*((*data)++) = '\\';
		type |= (*n[1] == DEFAULT && *val == '*') * WILD;
		*((*data)++) = *val;
		if (*(val++) == '\\')
			*((*data)++) = '\\';
	}
	return (type);
}

int	interpolate_var(int **n, char *str, t_list *vars, char **data)
{
	int		error;

	if (str[*n[0]] == '~')
		return (interpolation_value(n, str, vars, data));
	if ((str[*n[0] + 1] == '"' || str[*n[0] + 1] == '\'') && *n[1] == DEFAULT)
		return (0);
	if (!ft_isalnum(str[*n[0] + 1]) && str[*n[0] + 1] != '?'
		&& str[*n[0] + 1] != '_' && str[*n[0] + 1] != '*')
		*((*data)++) = str[*n[0]];
	else if (ft_isdigit(str[*n[0] + 1]) || str[*n[0] + 1] == '*')
		++(*n[0]);
	else if (str[*n[0] + 1] == '?')
		interpolation_error(n, data);
	else
		return (interpolation_value(n, str, vars, data));
	return (0);
}
