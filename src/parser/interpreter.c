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

int	setup_file_fds(t_list **node, t_cmd *cmd, int type)
{
	int	io;
	int	flags;

	io = !(type & (LESS | DLESS));
	flags = io * (O_WRONLY | O_CREAT | O_APPEND * (type == DGREAT)
		| O_TRUNC * (type == GREAT)) + !io * O_RDONLY;
	if (((t_token *)(*node)->next->content)->type == FILES
		&& ((t_token *)(*node)->next->next->content)->type == FILES)
		return (error_msg(1, E_AMBI, ((t_token *)(*node)->content)->data));
	*node = (*node)->next;
	if (type == DLESS && (!cmd->fd[io] || close(cmd->fd[io]) || 1))
		cmd->fd[io] = open(".tmp", flags, 0644);
	else if (!cmd->fd[io] || close(cmd->fd[io]) || 1)
		cmd->fd[io] = open(((t_token *)(*node)->content)->data,
				flags, 0644);
	if (cmd->fd[io] < 0)
		return (error_msg(errno, E_FILE, ((t_token *)(*node)->content)->data,
			strerror(errno)));
	return (0);
}

int	setup_file_redirections(t_list **node, t_cmd *cmd)
{
	int	type;
	int	error;

	error = 0;
	while (!error && *node && !(((t_token *)(*node)->content)->type
			& (LPAREN | RPAREN | OR | SEMI | OR_IF | AND_IF | END)))
	{
		type = ((t_token *)(*node)->content)->type;
		if (type & (LESS | DLESS | GREAT | DGREAT))
			error = setup_file_fds(node, cmd, type);
		*node = (*node)->next;
	}
	if (!access(".tmp", F_OK))
		unlink(".tmp");
	return (error);
}

int	parse_expr(t_list *node, t_list **vars, t_cmd *cmd, int valid)
{
	int	i;
	int	error;

	i = 0;
	error = 0;
	cmd->ior_start = NULL;
	while (!error && !(((t_token *)node->content)->type
			& (LPAREN | RPAREN | OR | SEMI | OR_IF | AND_IF | END)))
	{
		if (((t_token *)node->content)->type & (CMD | OPTS1 | OPTS2 | ARGS))
			cmd->opts[i++] = ((t_token *)node->content)->data;
		if (!(((t_token *)node->content)->type & BUILTIN))
			get_cmd_path(cmd->opts[0], cmd->path, *vars);
		if (((t_token *)node->content)->type == DLESS)
			error = get_heredoc(node->next->content, "> ", *vars);
		if (((t_token *)node->content)->type & (LESS | GREAT | DLESS | DGREAT)
			&& !cmd->ior_start)
			cmd->ior_start = node;
		node = node->next;
	}
	cmd->opts[i] = NULL;
	if (!valid)
		unlink(".tmp");
	return (error);
}

int	execute_cmd(t_list **vars, t_cmd *cmd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cmd->opts[0] && !ft_strcmp_ign(cmd->opts[0], "cd"))
		return (builtin_cd(cmd->opts, vars));
	if (cmd->opts[0] && !ft_strcmp_ign(cmd->opts[0], "echo"))
		return (builtin_echo(cmd->opts, vars));
	if (cmd->opts[0] && !ft_strcmp_ign(cmd->opts[0], "env"))
		return (builtin_env(cmd->opts, vars));
	if (cmd->opts[0] && !ft_strcmp(cmd->opts[0], "exit"))
		return (builtin_exit(cmd->opts, vars));
	if (cmd->opts[0] && !ft_strcmp(cmd->opts[0], "export"))
		return (builtin_export(cmd->opts, vars));
	if (cmd->opts[0] && !ft_strcmp_ign(cmd->opts[0], "pwd"))
		return (builtin_pwd(cmd->opts, vars));
	if (cmd->opts[0] && !ft_strcmp(cmd->opts[0], "unset"))
		return (builtin_unset(cmd->opts, vars));
	return (execve(cmd->path, cmd->opts, cmd->envs));
}

int	exec(t_list **vars, t_cmd *cmd)
{
	char	*s;

	if (cmd->fd[2] && (close(cmd->fd[2]) || 1))
		cmd->fd[2] = 0;
	if (setup_file_redirections(&(cmd->ior_start), cmd))
		return (1);
	if (((cmd->fd[0] && dup2(cmd->fd[0], STDIN_FILENO) < 0)
			|| (cmd->fd[1] && dup2(cmd->fd[1], STDOUT_FILENO) < 0))
		&& (cmd->fd[0] && close(cmd->fd[0]) || 1)
		&& (cmd->fd[1] && close(cmd->fd[1]) || 1))
		return (error_msg(errno, E_DUP2, strerror(errno)));
	if (cmd->fd[0] && (close(cmd->fd[0]) || 1))
		cmd->fd[0] = 0;
	if (cmd->fd[1] && (close(cmd->fd[1]) || 1))
		cmd->fd[1] = 0;
	if (!(cmd->path[0]) && cmd->opts[0] && !is_builtin(cmd->opts[0]))
		return (error_msg(127, E_CMDE, cmd->opts[0]));
	if (cmd->path[0] && access(cmd->path, X_OK))
		return (error_msg(126, E_CMDS, cmd->opts[0], strerror(errno)));
	if (cmd->path[0] && access(cmd->path, R_OK))
		return (error_msg(126, E_CMDS, cmd->opts[0], strerror(errno)));
	return (execute_cmd(vars, cmd));
}

int	exec_builtin(t_list **vars, t_cmd *cmd)
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
		return (error_msg(errno, E_DUPE, strerror(errno)));
	}
	status = exec(vars, cmd);
	if (dup2(fd[0], STDIN_FILENO) < 0 || dup2(fd[1], STDOUT_FILENO) < 0)
		status = error_msg(errno, E_DUP2, strerror(errno));
	close(fd[0]);
	close(fd[1]);
	return (status);
}

int	proto(t_list *node, t_list **vars, t_cmd *cmd, int flags)
{
	int	status;

	if (parse_expr(node, vars, cmd, flags & 0x1) || !(flags & 1))
		return (1);
	if (!cmd->child && is_builtin(cmd->opts[0]))
		return (exec_builtin(vars, cmd));
	ft_swap(&cmd->fd[0], &cmd->fd[2]);
	cmd->pid = fork();
	if (cmd->pid < 0 || (!cmd->pid && exec(vars, cmd)))
		return (!cmd->pid || error_msg(errno, E_FORK, strerror(errno)));
	if (cmd->fd[0] && (close(cmd->fd[0]) || 1))
		cmd->fd[0] = 0;
	if (cmd->fd[1] && (close(cmd->fd[1]) || 1))
		cmd->fd[1] = 0;
	if (!(flags & 2) || !cmd->pid)
		return (0);
	if (cmd->fd[2] && (close(cmd->fd[2]) || 1))
		cmd->fd[2] = 0;
	if (waitpid(cmd->pid, &status, 0) < 0)
		return (error_msg(errno, E_WAIT, strerror(errno)));
	while (cmd->child && cmd->child--)
		if (waitpid(-1, NULL, 0) < 0)
			return (error_msg(errno, E_WAIT, strerror(errno)));
	return (error_msg(WEXITSTATUS(status) | WTERMSIG(status), ""));
}

int	evaluate_expr(t_ast *ast, t_list **vars, t_cmd *cmd, int valid)
{
	if (((t_token *)ast->expr->content)->type == OR
		&& ((t_token *)ast->left->expr->content)->type == OR)
		if (evaluate_expr(ast->left, vars, cmd, valid))
			return (1);
	if (ast->expr && ((t_token *)ast->expr->content)->type == OR)
	{
		if (pipe(cmd->fd))
			return (error_msg(errno, E_PIPE, strerror(errno)));
		cmd->child++;
		if (proto(ast->left->expr, vars, cmd, valid) && !cmd->pid)
			return (1);
		ast->expr = NULL;
		if (ast->right)
			ast->expr = ast->right->expr;
	}
	return (0);
}

int	interpret_ast(t_ast *ast, t_list **vars, t_cmd *cmd, int valid)
{
	if (((t_token *)ast->expr->content)->type == AND_IF)
		return ((interpret_ast(ast->left, vars, cmd, valid) && valid)
			|| interpret_ast(ast->right, vars, cmd, valid));
	if (((t_token *)ast->expr->content)->type == OR_IF)
		return (interpret_ast(ast->left, vars, cmd, valid)
			&& interpret_ast(ast->right, vars, cmd, valid));
	if (evaluate_expr(ast, vars, cmd, valid))
		return (1);
	if (cmd->pid && ast->expr)
		if (proto(ast->expr, vars, cmd, 2 | valid))
			return (1);
	return (0);
}
