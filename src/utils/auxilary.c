/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auxilary.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snagulap <snagulap@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 11:45:11 by snagulap          #+#    #+#             */
/*   Updated: 2023/08/08 12:05:00 by snagulap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

char	*prompt_line(char *prompt)
{
	char	*ln;
	char	*line;

	if (isatty(fileno(stdin)))
		line = readline(prompt);
	else
	{
		ln = get_next_line(fileno(stdin));
		line = ft_strtrim(ln, "\n");
		free(ln);
	}
	return (line);
}
