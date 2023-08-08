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

void	clear(void *content)
{
	t_token	*token;

	token = (t_token *)content;
	if (token->data)
		free(token->data);
	free(content);
}

void	setup_termios(struct termios *termios)
{
	struct termios	new_termios;

	tcgetattr(STDOUT_FILENO, termios);
	new_termios = *termios;
	new_termios.c_lflag &= ~ECHOCTL;
	tcsetattr(STDOUT_FILENO, TCSAFLUSH, &new_termios);
}

int	semi_syntax_handler(char *line)
{
	int		*s;

	s = (int []){DEFAULT, 0};
	while (*line && !(s[1] && (token_type(line)
				& (RPAREN | SEMI | OR | AND | OR_IF | AND_IF))))
	{
		if (s[0] != SQUOTE && *line == '\\' && !(*(line + 1)))
			break ;
		if (s[0] != SQUOTE && *line == '\\')
			line++;
		else if ((*line == '\'' && s[0] == SQUOTE)
			|| (*line == '"' && s[0] == DQUOTE))
			s[0] = DEFAULT;
		else if ((*line == '\'' || *line == '"') && s[0] == DEFAULT)
			s[0] = (*line != '"') * SQUOTE + (*line == '"') * DQUOTE;
		else if (s[0] == DEFAULT && token_type(line) != SPACES)
			s[1] = token_type(line) == SEMI;
		line++;
	}
	if (*line == '\\' && !(*(line + 1)))
		ft_dprintf(2, E_ESCP);
	else if (*line)
		ft_dprintf(2, E_SYTX,
			(char []){*line, *line * (token_type(line) >= DLESS), 0});
	return ((*line == '\\' && !(*(line + 1))) || *line);
}

char	*semi_line_iterator(char *line)
{
	int	i;
	int	state;

	i = -1;
	state = DEFAULT;
	while (line[++i] && (line[i] != ';' || state != DEFAULT))
	{
		if (state != SQUOTE && line[i] == '\\')
			i++;
		else if ((line[i] == '\'' && state == SQUOTE)
			|| (line[i] == '"' && state == DQUOTE))
			state = DEFAULT;
		else if ((line[i] == '\'' || line[i] == '"') && state == DEFAULT)
			state = (line[i] != '"') * SQUOTE + (line[i] == '"') * DQUOTE;
	}
	return (&line[i - !line[i]]);
}
