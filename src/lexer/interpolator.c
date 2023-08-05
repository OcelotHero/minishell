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

int	interpolation_length(int **n, char *str, t_list *var_list)
{
	int			count;
	char		*val;

	if ((str[*n[0] + 1] == '"' || str[*n[0] + 1] == '\'')  && *n[1] == DEFAULT)
		return (0);
	if (!ft_isalnum(str[*n[0] + 1]) && str[*n[0] + 1] != '?'
		&& str[*n[0] + 1] != '_')
		return (1);
	if (ft_isdigit(str[*n[0] + 1]) || str[*n[0] + 1] == '?')
		return ((str[++(*n[0])] == '?') * n_digit(g_errno));
	val = var_value(&str[*n[0] + 1], var_list);
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

void	interpolation_value(int *i, char *str, t_list *var_list, char **data)
{
	char	*val;

	val = var_value(&str[*i + 1], var_list);
	while (ft_isalnum(str[*i + 1]) || str[*i + 1] == '_')
		(*i)++;
	if (!val)
		return ;
	while (*val)
	{
		if (*val == '\'' || *val == '"')
			*((*data)++) = '\\';
		*((*data)++) = *(val++);
		if (*(val - 1) == '\\')
			*((*data)++) = '\\';
	}
}

void	interpolate_var(int **n, char *str, t_list *var_list, char **data)
{
	int		error;

	if ((str[*n[0] + 1] == '"' || str[*n[0] + 1] == '\'') && *n[1] == DEFAULT)
		return ;
	if (!ft_isalnum(str[*n[0] + 1]) && str[*n[0] + 1] != '?'
		&& str[*n[0] + 1] != '_')
		*((*data)++) = str[*n[0]];
	else if (ft_isdigit(str[*n[0] + 1]) || str[*n[0] + 1] == '?')
	{
		if (str[*n[0] + 1] == '?')
		{
			error = g_errno;
			*data += n_digit(error);
			while (error >= 10)
			{
				*(--(*data)) = (error % 10) + '0';
				error /= 10;
			}
			*(--(*data)) = error + '0';
		}
		*data += (str[++(*n[0])] == '?') * n_digit(g_errno);
	}
	else
		interpolation_value(n[0], str, var_list, data);
}
