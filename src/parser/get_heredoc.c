/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snagulap <snagulap@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 18:28:32 by rraharja          #+#    #+#             */
/*   Updated: 2023/08/04 14:47:21 by snagulap         ###   ########.fr       */
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
		ft_dprintf(fd, "%d", errno);
	else
	{
		val = var_value(&str[*i + 1], vars);
		while (ft_isalnum(str[*i + 1]) || str[*i + 1] == '_')
			(*i)++;
		if (val)
			ft_dprintf(fd, "%s", val);
	}
}

static int	process_line(int fd, t_token *token, char *ln, t_list *vars)
{
	int		i;

	if (!ln || ln[0] == 3
		|| !ft_strncmp(ln, token->data, ft_strlen(token->data) + 1))
		return (1);
	i = -1;
	while (ln[++i])
	{
		if (token->type & QUOT)
			write(fd, &ln[i], 1);
		else if (ln[i] == '$')
			expand_var(fd, &i, ln, vars);
		else
		{
			if ((ln[i] == '\\') && (ln[i + 1] == '$' || ln[i + 1] == '\\'))
				i++;
			else if (ln[i] == '\\' && !ln[i + 1])
				break ;
			write(fd, &ln[i], 1);
		}
	}
	if (!ln[i])
		write(fd, "\n", 1);
	free(ln);
	return (0);
}

int	event(void)
{
	return (0);
}

void	int_handler(int signo)
{
	char	end;

	end = '\x03';
	errno = signo;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line(&end, 0);
	rl_redisplay();
	rl_done = 1;
}

/**
 * Creates a temporary file to be used as input file for pipex, when here_doc
 * is invoked, stopping only when limiter is reached.
 */
int	get_heredoc(t_token *token, char *prompt, t_list *vars)
{
	int		fd;
	char	*ln;

	errno = 0;
	rl_event_hook = event;
	signal(SIGINT, int_handler);
	fd = open(".tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (error_msg(errno, E_FILE, ".tmp", strerror(errno)));
	while (1)
	{
		ln = readline(prompt);
		if (process_line(fd, token, ln, vars))
			break ;
	}
	close(fd);
	if (!ln && errno)
		return (error_msg(errno, E_MLOC, strerror(errno))
			&& (unlink(".tmp") || 1));
	if (!ln)
		ft_dprintf(2, E_EOFW, token->data);
	if (ln)
		free(ln);
	return (0);
}
