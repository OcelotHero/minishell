/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 20:20:43 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/12 20:20:43 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	setup_file_redirections(t_list *node, t_cmd *cmd, int valid)
{
	int		io;
	int		type;
	int		flags;

	while (valid && node && !(((t_token *)node->content)->type
			& (LPAREN | RPAREN | OR | SEMI | OR_IF | AND_IF | END)))
	{
		type = ((t_token *)node->content)->type;
		if (type & (LESS | DLESS | GREAT | DGREAT))
		{
			io = !(type & (LESS | DLESS));
			flags = io * (O_WRONLY | O_CREAT | O_APPEND * (type == DGREAT)
					| O_TRUNC * (type == GREAT)) + !io * O_RDONLY;
			if (type == DLESS && (!cmd->fd[io] || !close(cmd->fd[io])))
				cmd->fd[io] = open(".tmp", flags, 0644);
			else if (!cmd->fd[io] || !close(cmd->fd[io]))
				cmd->fd[io] = open(((t_token *)node->next->content)->data,
						flags, 0644);
			if (cmd->fd[io] < 0)
				return (1);
			node = node->next;
		}
		node = node->next;
	}
	return (!access(".tmp", F_OK) && unlink(".tmp"));
}

int	parse_expr(t_list *node, t_list **vars, t_cmd *cmd)
{
	int	i;
	int	error;

	i = 0;
	error = 0;
	while (!error && !(((t_token *)node->content)->type
			& (LPAREN | RPAREN | OR | SEMI | OR_IF | AND_IF | END)))
	{
		if (((t_token *)node->content)->type & (CMD | OPTS1 | OPTS2 | ARGS))
			cmd->opts[i++] = ((t_token *)node->content)->data;
		if (((t_token *)node->content)->type & CMD)
			get_cmd_path(cmd->opts[0], cmd->path, *vars);
		if (((t_token *)node->content)->type == DLESS)
			error = get_heredoc(node->next->content, "> ", *vars);
		if (((t_token *)node->content)->type & (LESS | GREAT | DLESS | DGREAT)
			&& !cmd->ior_start)
			cmd->ior_start = node;
		node = node->next;
	}
	cmd->opts[i] = NULL;
	return (error);
}

int	execute_cmd(t_list **vars, t_cmd *cmd)
{
	if (!ft_strcmp(cmd->opts[0], "cd"))
		return (builtin_cd(cmd->opts, vars));
	if (!ft_strcmp(cmd->opts[0], "echo"))
		return (builtin_echo(cmd->opts, vars));
	if (!ft_strcmp(cmd->opts[0], "env"))
		return (builtin_env(cmd->opts, vars));
	if (!ft_strcmp(cmd->opts[0], "exit"))
		return (builtin_exit(cmd->opts, vars));
	if (!ft_strcmp(cmd->opts[0], "export"))
		return (builtin_export(cmd->opts, vars));
	if (!ft_strcmp(cmd->opts[0], "pwd"))
		return (builtin_pwd(cmd->opts, vars));
	if (!ft_strcmp(cmd->opts[0], "unset"))
		return (builtin_unset(cmd->opts, vars));
	return (execve(cmd->path, cmd->opts, NULL));
}

int	exec(t_list **vars, t_cmd *cmd, int valid)
{
	char	*s;

	if (cmd->fd[2])
		close(cmd->fd[2]);
	if (setup_file_redirections(cmd->ior_start, cmd, valid))
		return (-1);
	if (((cmd->fd[0] && dup2(cmd->fd[0], STDIN_FILENO) < 0)
			|| (cmd->fd[1] && dup2(cmd->fd[1], STDOUT_FILENO) < 0))
		&& (cmd->fd[0] && close(cmd->fd[0]) || 1)
		&& (cmd->fd[1] && close(cmd->fd[1]) || 1))
		return (-1);
	if (cmd->fd[0])
		close(cmd->fd[0]);
	if (cmd->fd[1])
		close(cmd->fd[1]);
	if (!valid || execute_cmd(vars, cmd))
		return (-1);
	return (0);
}

int	p1(t_list **vars, t_cmd *cmd, int valid)
{
	int	*fd;
	int	status;

	fd = (int []){dup(STDIN_FILENO), dup(STDOUT_FILENO)};
	if (fd[0] < 0 || fd[1] < 0)
	{
		if (fd[0] > 0)
			close(fd[0]);
		if (fd[1] > 0)
			close(fd[1]);
		return (-1);
	}
	status = exec(vars, cmd, valid);
	if (dup2(fd[0], STDIN_FILENO) < 0 || dup2(fd[1], STDOUT_FILENO) < 0)
		status = -1;
	close(fd[0]);
	close(fd[1]);
	return (status);
}

int	p(t_list *node, t_list **vars, t_cmd *cmd, int flags)
{
	int	status;

	if (parse_expr(node, vars, cmd))
		return (1);
	if (!cmd->child && is_builtin(cmd->opts[0]))
		return (p1(vars, cmd, flags & 0x1));
	ft_swap(&cmd->fd[0], &cmd->fd[2]);
	cmd->pid = fork();
	if (cmd->pid == -1 || (!cmd->pid && exec(vars, cmd, flags & 0x1)))
		return (-1);
	if (cmd->fd[1] && (close(cmd->fd[1]) || 1))
		cmd->fd[1] = 0;
	if (!(flags & 0x2))
		return (0);
	if (cmd->fd[0])
		close(cmd->fd[0]);
	if (cmd->fd[2])
		close(cmd->fd[2]);
	waitpid(cmd->pid, &status, 0);
	while (cmd->child--)
		waitpid(-1, NULL, 0);
	return (status);
}

int	proto(t_ast *ast, t_list **vars, t_cmd *cmd, int valid)
{
	int	flags;

	if (((t_token *)ast->expr->content)->type & (SEMI | OR)
		&& ((t_token *)ast->left->expr->content)->type & (SEMI | OR))
		proto(ast->left, vars, cmd, valid);
	if (ast->expr && ((t_token *)ast->expr->content)->type == OR)
	{
		if (pipe(cmd->fd))
			return (-1);
		cmd->child++;
	}
	if (ast->expr && ((t_token *)ast->expr->content)->type & (SEMI | OR))
	{
		flags = (((t_token *)ast->expr->content)->type == SEMI) << 1 | valid;
		p(ast->left->expr, vars, cmd, flags);
		ast->expr = NULL;
		if (ast->right)
			ast->expr = ast->right->expr;
	}
	return (0);
}

int	evaluate_expr(t_ast *ast, t_list **vars, int valid)
{
	int		status;
	t_cmd	cmd;
	int		*fd;

	cmd = (t_cmd){0, {0, 0, 0}, 0, "", NULL, NULL};
	proto(ast, vars, &cmd, valid);
	if (ast->expr)
		p(ast->expr, vars, &cmd, 1 << 1 | valid);
	return (0);
}

int	interpret_ast(t_ast *ast, t_list **vars, int valid)
{
	if (((t_token *)ast->expr->content)->type == AND_IF)
		return (interpret_ast(ast->left, vars, valid)
			|| interpret_ast(ast->right, vars, valid));
	if (((t_token *)ast->expr->content)->type == OR_IF)
		return (interpret_ast(ast->left, vars, valid)
			&& interpret_ast(ast->right, vars, valid));
	return (evaluate_expr(ast, vars, valid));
}
