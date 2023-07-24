/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 01:19:54 by rraharja          #+#    #+#             */
/*   Updated: 2022/10/18 01:19:54 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"

int	ft_fprintf(int fd, const char *format, ...)
{
	int		i;
	int		total;
	va_list	args;

	va_start(args, format);
	total = 0;
	while (format && *format)
	{
		i = 0;
		while (format[i] && format[i] != '%')
			write(fd, &format[i++], 1);
		total += i;
		format += i;
		total += ft_parse(fd, &format, args);
	}
	va_end(args);
	return (total);
}
