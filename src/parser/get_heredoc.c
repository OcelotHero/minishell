/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 18:28:32 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/18 16:05:37 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/**
 * Creates a temporary file to be used as input file for pipex, when here_doc
 * is invoked, stopping only when limiter is reached.
 */
int	get_heredoc(char *limiter, char *prompt)
{
	int		fd;
	char	*line;

	fd = open(".tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (1);
	while (1)
	{
		line = readline(prompt);
		if (!line || !ft_strncmp(line, limiter, ft_strlen(limiter) + 1))
			break ;
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
	}
	close(fd);
	if (!line)
	{
		unlink(".tmp");
		return (1);
	}
	free(line);
	return (0);
}
