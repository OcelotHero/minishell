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

#include "minishell.h"

int	g_errno = 0;

int	processor(char *line, t_list **tokens, t_cmd *cmd, t_list **vars)
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
		res = (interpret_ast(ast, vars, cmd, res == 0) || res);
	ast_clear(&ast, NULL);
	return (res);
}

int	process_line_input(char *line, t_list **vars, char **envs)
{
	int		n[3];
	char	*sep;
	t_cmd	cmd;
	t_list	*tokens;

	n[0] = 0;
	tokens = NULL;
	cmd = (t_cmd){1, {0, 0, 0}, 0, "", {NULL}, .envs = envs, NULL};
	while (*line && cmd.pid && !n[0])
	{
		sep = semi_line_iterator(line);
		n[2] = *(sep + 1);
		*(sep + 1) = '\0';
		n[1] = processor(line, &tokens, &cmd, vars);
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

int	setup_env_vars(t_list **vars, char **envs)
{
	char	*str;
	char	buf[BUFSIZ];
	t_list	*node;

<<<<<<< HEAD
	while (*envs)
=======
	ast = NULL;
	tokens = NULL;
	cmd = (t_cmd){1, {0, 0, 0}, 0, "", {NULL}, .envs = envs, NULL};
	stop = 0;
	while (*line && cmd.pid && !stop)
>>>>>>> 97468d3b508126543b10eec73f7b4f912ed7f22f
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

int	minishell_non_interactive(char **args, t_list **vars, char **envs)
{
	char	*line;

	line = ft_strrchr(args[2], '\n');
	if (line)
		line[0] = '\0';
	line = args[2];
	if (line && *line && !semi_syntax_handler(line))
		process_line_input(line, vars, envs);
	return (g_errno);
}

int	minishell_interactive(t_list **vars, char **envs)
{
	char			**line;
	struct termios	termios;

	line = (char *[]){"", NULL};
	while (line[0])
	{
		setup_signals();
		setup_termios(&termios);
		line[0] = prompt_line("$minishell> ");
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
			if (process_line_input(line[0], vars, envs))
				break ;
		}
	}
	free(line[1]);
	return (g_errno);
}
