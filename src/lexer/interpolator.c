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

int	interpolation_length(int *i, char *str, t_list *var_list)
{
	int		count;
	char	*val;

	if (!ft_isalnum(str[*i + 1]) && str[*i + 1] != '?' && str[*i + 1] != '_')
		return (1);
	if (ft_isdigit(str[*i + 1]) || str[*i + 1] == '?')
		return ((str[++(*i)] == '?') * n_digit(errno));
	val = var_value(&str[*i + 1], var_list);
	while (ft_isalnum(str[*i + 1]) || str[*i + 1] == '_')
		(*i)++;
	if (!val)
		return (0);
	count = 0;
	while(*val)
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
	while(*val)
	{
		if (*val == '\'' || *val == '"')
			*((*data)++) = '\\';
		*((*data)++) = *(val++);
		if (*(val - 1) == '\\')
			*((*data)++) = '\\';
	}
}

void	interpolate_var(int *i, char *str, t_list *var_list, char **data)
{
	int		error;

	if (!ft_isalnum(str[*i + 1]) && str[*i + 1] != '?' && str[*i + 1] != '_')
		*((*data)++) = str[*i];
	else if (ft_isdigit(str[*i + 1]) || str[*i + 1] == '?')
	{
		if (str[*i + 1] == '?')
		{
			error = errno;
			*data += n_digit(errno);
			while (error >= 10)
			{
				*(--(*data)) = (error % 10) + '0';
				error /= 10;
			}
			*(--(*data)) = error + '0';
		}
		*data += (str[++(*i)] == '?') * n_digit(errno);
	}
	else
		interpolation_value(i, str, var_list, data);
}
