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

static void	expand_var(int fd, int *i, char *str, t_list *vars)
{
	char	*val;

	if (!ft_isalnum(str[*i + 1]) && str[*i + 1] != '?' && str[*i + 1] != '_')
		write(fd, &str[*i], 1);
	else if (ft_isdigit(str[*i + 1]))
		(*i)++;
	else if (str[*i + 1] == '?' && ++(*i))
		ft_fprintf(fd, "%d", errno);
	else
	{
		val = var_value(&str[*i + 1], vars);
		while (ft_isalnum(str[*i + 1]) || str[*i + 1] == '_')
			(*i)++;
		if (val)
			ft_fprintf(fd, "%s", val);
	}
}

static int	process_line(int fd, t_token *token, char *line, t_list *vars)
{
	int		i;

	if (!line || line[0] == 1
		|| !ft_strncmp(line, token->data, ft_strlen(token->data) + 1))
		return (1);
	i = -1;
	while (line[++i])
	{
		if (token->type & QUOT)
			write(fd, &line[i], 1);
		else if (line[i] == '$')
			expand_var(fd, &i, line, vars);
		else
		{
			if (line[i] == '\\' && (line[i + 1] == '$' || line[i + 1] == '\\'))
				i++;
			else if (line[i] == '\\' && !line[i + 1])
				break ;
			write(fd, &line[i], 1);
		}
	}
	if (!line[i])
		write(fd, "\n", 1);
	free(line);
	return (0);
}

int	event(void)
{
	return (0);
}

void	int_handler(int signo)
{
	errno = signo;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("\x01", 0);
	rl_done = 1;
}

/**
 * Creates a temporary file to be used as input file for pipex, when here_doc
 * is invoked, stopping only when limiter is reached.
 */
int	get_heredoc(t_token *token, char *prompt, t_list *vars)
{
	int		fd;
	int		stop;
	char	*line;

	rl_event_hook = event;
	signal(SIGINT, int_handler);
	fd = open(".tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (1);
	stop = 0;
	while (!stop)
	{
		line = readline(prompt);
		stop = process_line(fd, token, line, vars);
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
