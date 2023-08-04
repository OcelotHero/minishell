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

#define E_SYTX "minishell: syntax error near unexpected token `%s'\n"
#define E_ESCP "\n"

void	clear(void *content)
{
	t_token	*token;

	token = (t_token *)content;
	// printf("%06x, `%s`\n", token->type, token->data);
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
	int		state;
	int		res;
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
	while (*line && cmd.pid)
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
		if (!tokenize(&tokens, line, *vars))
		{
			postprocess_tokens(tokens);
			trim_tokens(&tokens);
			tmp = tokens;
			if (((t_token *)tmp->content)->type != END)
			{
				res = expr(&tmp, &ast);
				tkn = token_str(((t_token *)tmp->content)->type);
				if (!(*tkn))
					tkn = "newline";
				if (res == 2)
					ft_dprintf(2, E_SYTX, tkn);
				if (ast && res != 1)
					interpret_ast(ast, vars, &cmd, res == 0);
				ast_clear(&ast, NULL);
			}
		}

		ft_lstclear(&tokens, clear);
		*(sep + 1) = c;
		line = sep + 1;
	}
	return (cmd.pid <= 0);
}

void	setup_termios(struct termios *termios)
{
	struct termios	new_termios;

	tcgetattr(STDOUT_FILENO, termios);
	new_termios = *termios;
	new_termios.c_lflag &= ~ECHOCTL;
	tcsetattr(STDOUT_FILENO, TCSAFLUSH, &new_termios);
}

int	g_errno = 0;

int	main(int narg, char **args, char **envs)
{
	int		res;
	char	buf[BUFSIZ];
	char	*dir;
	char	**line;
	t_list	*vars;
	struct termios	termios;


	vars = NULL;
	line = (char *[]){"", NULL};

	int i = 0;
	while (envs[i])
		ft_lstadd_front(&vars, ft_lstnew(ft_strdup(envs[i++])));
	dir = getcwd(buf, BUFSIZ);
	ft_lstadd_front(&vars, ft_lstnew(ft_strdup("SHLVL=1")));
	ft_lstadd_front(&vars, ft_lstnew(ft_strjoin("PWD=", dir)));


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
			// line[0] = readline("$minishell> ");
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
