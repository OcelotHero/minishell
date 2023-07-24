/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_none.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 15:07:18 by rraharja          #+#    #+#             */
/*   Updated: 2022/10/23 15:27:14 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"

size_t	ft_fprint_none(int fd, t_pconfig *config, const char **format,
	char *start)
{
	*format = start + 1;
	return (ft_fprint_percent(fd, config));
}
