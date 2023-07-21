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
	int		j;
	t_list	*node;

	if (!ft_isalnum(str[*i + 1]) && str[*i + 1] != '?' && str[*i + 1] != '_')
		return (1);
	if (ft_isdigit(str[*i + 1]) || str[*i + 1] == '?')
		return ((str[++(*i)] == '?') * n_digit(errno));
	node = var_list;
	while (node)
	{
		j = 0;
		while (((char *)node->content)[j] == str[*i + 1 + j])
			j++;
		if (((char *)node->content)[j] == '=' &&
			!ft_isalnum(str[*i + 1 + j]) && str[*i + 1 + j] != '_')
		{
			*i += j;
			return (ft_strlen((char *)node->content) - j - 1);
		}
		node = node->next;
	}
	while (ft_isalnum(str[*i + 1]) || str[*i + 1] == '_')
		(*i)++;
	return (0);
}

void	interpolation_value(int *i, char *str, t_list *var_list, char **data)
{
	int		j;
	int		n;
	t_list	*node;

	node = var_list;
	while (node)
	{
		j = 0;
		while (((char *)node->content)[j] == str[*i + 1 + j])
			j++;
		if (((char *)node->content)[j] == '=' &&
			!ft_isalnum(str[*i + 1 + j]) && str[*i + 1 + j] != '_')
		{
			*i += j;
			n = ft_strlen((char *)node->content) - j - 1;
			ft_memcpy(*data, &((char *)node->content)[j + 1], n);
			*data += n;
			return ;
		}
		node = node->next;
	}
	while (ft_isalnum(str[*i + 1]) || str[*i + 1] == '_')
		(*i)++;
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
