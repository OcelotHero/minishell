/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 02:56:31 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/07 02:56:31 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"
#include "signals.h"
#include "ft_dprintf.h"
#include <termios.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "get_next_line.h"

int	g_errno = 0;

void	clear(void *content)
{
	t_token	*token;

	token = (t_token *)content;
	if (token->data)
		free(token->data);
	free(content);
}

static int	semi_syntax_handler(char *line)
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

static int	process(char *line, t_list **vars, char **envs)
{
	int		i;
	int		stop;
	int		state;
	int		res;
	int		ret;
	char	c;
	char	*sep;
	char	*tkn;
	t_cmd	cmd;
	t_ast	*ast;
	t_list	*tmp;
	t_list	*tokens;

	ast = NULL;
	tokens = NULL;
	cmd = (t_cmd){1, {0, 0, 0}, 0, "", NULL, .envs = envs, NULL};
	stop = 0;
	while (*line && cmd.pid && !stop)
	{
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
		sep = &line[i - !line[i]];
		c = *(sep + 1);
		*(sep + 1) = '\0';
		ret = 1;
		if (!tokenize(&tokens, line, *vars))
		{
			postprocess_tokens(tokens);
			trim_tokens(&tokens);
			tmp = tokens;
			if (((t_token *)tmp->content)->type != END)
			{
				res = expr(&tmp, &ast);
				g_errno = res;
				tkn = token_str(((t_token *)tmp->content)->type);
				if (!(*tkn))
					tkn = "newline";
				if (res == 2)
					ft_dprintf(2, E_SYTX, tkn);
				if (ast && res != 1)
					ret = interpret_ast(ast, vars, &cmd, res == 0);
				ast_clear(&ast, NULL);
			}
			if (cmd.pid && !access(".tmp", F_OK))
				unlink(".tmp");
		}
		if (cmd.opts[0] && ft_strcmp(cmd.opts[0], "exit"))
			g_errno *= ret;
		else if (cmd.opts[0] && !ft_strcmp(cmd.opts[0], "exit"))
		{
			if (!ret)
				stop = 1;
			else
				g_errno *= ret;
		}
		ft_lstclear(&tokens, clear);
		*(sep + 1) = c;
		line = sep + 1;
		if (res)
			g_errno = res;
	}
	return (cmd.pid <= 0 || stop);
}

void	setup_termios(struct termios *termios)
{
	struct termios	new_termios;

	tcgetattr(STDOUT_FILENO, termios);
	new_termios = *termios;
	new_termios.c_lflag &= ~ECHOCTL;
	tcsetattr(STDOUT_FILENO, TCSAFLUSH, &new_termios);
}

int	main(int narg, char **args, char **envs)
{
	int				res;
	int				i;
	char			buf[BUFSIZ];
	char			*dir;
	char			**line;
	t_list			*vars;
	struct termios	termios;

	vars = NULL;
	line = (char *[]){"", NULL};
	i = 0;
	while (envs[i])
		ft_lstadd_front(&vars, ft_lstnew(ft_strdup(envs[i++])));
	if (narg == 3)
	{
		line[0] = ft_strrchr(args[2], '\n');
		if (line[0])
			line[0][0] = '\0';
		line[0] = args[2];
		if (line[0] && *line[0] && !semi_syntax_handler(line[0]))
			process(line[0], &vars, envs);
		ft_lstclear(&vars, free);
		return (g_errno);
	}
	else
	{
		while (line[0])
		{
			setup_signals();
			setup_termios(&termios);
			if (isatty(fileno(stdin)))
				line[0] = readline("$minishell> ");
			else
			{
				char *ln;
				ln = get_next_line(fileno(stdin));
				line[0] = ft_strtrim(ln, "\n");
				free(ln);
			}
			signal(SIGINT, SIG_IGN);
			if (line[0] && *line[0])
			{
				if (!line[1] || ft_strcmp(line[0], line[1]))
					add_history(line[0]);
				if (line[1])
					free(line[1]);
				line[1] = line[0];
				if (semi_syntax_handler(line[0]))
					continue ;
				if (process(line[0], &vars, envs))
					break ;
			}
		}
		free(line[1]);
		ft_lstclear(&vars, free);
		return (g_errno);
	}
}
