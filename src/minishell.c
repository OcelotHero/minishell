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

int	p(char *line, t_list **tokens, t_cmd *cmd, t_list **vars)
{
	int		res;
	char	*tkn;
	t_ast	*ast;
	t_list	*tmp;

	if (tokenize(tokens, line, *vars) || postprocess(*tokens) || trim(tokens))
		return (1);
	res = 1;
	ast = NULL;
	tmp = *tokens;
	if (((t_token *)tmp->content)->type == END)
		return (res);
	res = expr(&tmp, &ast);
	if (res == 1)
		error_msg(errno, E_MLOC, strerror(errno));
	res += 2 * (!res && (((t_token *)tmp->content)->type & ~(END | OR)));
	tkn = token_str(((t_token *)tmp->content)->type);
	if (!(*tkn))
		tkn = ((t_token *)tmp->content)->data;
	if (res == 2)
		error_msg(2, E_SYTX, tkn);
	if (ast && res != 1)
		res = interpret_ast(ast, vars, cmd, res == 0);
	ast_clear(&ast, NULL);
	return (res);
}

char	*pr(char *line)
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

static int	process(char *line, t_list **vars, char **envs)
{
	int		n[3];
	char	*sep;
	t_cmd	cmd;
	t_list	*tokens;

	n[0] = 0;
	tokens = NULL;
	cmd = (t_cmd){1, {0, 0, 0}, 0, "", NULL, .envs = envs, NULL};
	while (*line && cmd.pid && !n[0])
	{
		sep = pr(line);
		n[2] = *(sep + 1);
		*(sep + 1) = '\0'; 
		n[1] = p(line, &tokens, &cmd, vars);
		if (cmd.pid && !access(".tmp", F_OK))
			unlink(".tmp");
		if (cmd.opts[0] && (ft_strcmp(cmd.opts[0], "exit") || n[1]))
			g_errno *= n[1];
		else if (cmd.opts[0] && !ft_strcmp(cmd.opts[0], "exit") && !n[1])
			n[0] = 1;
		ft_lstclear(&tokens, clear);
		*(sep + 1) = n[2];
		line = sep + 1;
	}
	return (cmd.pid <= 0 || n[0]);
}

void	setup_termios(struct termios *termios)
{
	struct termios	new_termios;

	tcgetattr(STDOUT_FILENO, termios);
	new_termios = *termios;
	new_termios.c_lflag &= ~ECHOCTL;
	tcsetattr(STDOUT_FILENO, TCSAFLUSH, &new_termios);
}

int	setup_env_vars(t_list **vars, char **envs)
{
	char	*str;
	char	buf[BUFSIZ];
	t_list	*node;

	while (*envs)
	{
		str = ft_strdup(*(envs++));
		node = ft_lstnew(str);
		if (!str || !node)
		{
			if (str)
				free(str);
			return (error_msg(errno, E_MLOC, strerror(errno)));
		}
		ft_lstadd_front(vars, node);
	}
	str = getcwd(buf, BUFSIZ);
	if (builtin_unset((char *[]){"unset", "OLDPWD", NULL}, vars) || !str)
		return (error_msg(errno, E_PWDV, strerror(errno)));
	str = ft_strjoin("PWD=", str);
	if (!str)
		return (error_msg(errno, E_MLOC, strerror(errno)));
	builtin_export((char *[]){"export", str, NULL}, vars);
	free(str);
	return (0);
}

int	setup_env_vars(t_list **vars, char **envs)
{
	char	*str;
	char	buf[BUFSIZ];
	t_list	*node;

	while (*envs)
	{
		str = ft_strdup((*envs)++);
		node = ft_lstnew(str);
		if (!str || !node)
		{
			if (str)
				free(str);
			return (error_msg(errno, E_MLOC, strerror(errno)));
		}
		ft_lstadd_front(vars, node);
	}
	str = getcwd(buf, BUFSIZ);
	if (builtin_unset((char *[]){"unset", "OLDPWD", NULL}, vars) || !str)
		return (error_msg(errno, E_PWDV, strerror(errno)));
	str = ft_strjoin("PWD=", str);
	if (!str)
		return (error_msg(errno, E_MLOC, strerror(errno)));
	builtin_export((char *[]){"export", str, NULL}, vars);
	free(str);
	return (0);
}

int	main(int narg, char **args, char **envs)
{
	int				res;
	char			*dir;
	char			**line;
	t_list			*vars;
	struct termios	termios;

	vars = NULL;
	line = (char *[]){"", NULL};

	setup_env_vars(&vars, envs);
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
