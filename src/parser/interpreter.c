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

	while (node && !(((t_token *)node->content)->type
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
			else if (valid && (!cmd->fd[io] || !close(cmd->fd[io])))
				cmd->fd[io] = open(((t_token *)node->next->content)->data,
						flags, 0644);
			if (cmd->fd[io] < 0)
				return (1);
		}
		node = node->next;
	}
	return (!access(".tmp", F_OK) && unlink(".tmp"));
}

int	parse_expr(t_list *node, t_list **var_list, t_cmd *cmd, int valid)
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
		if (((t_token *)node->content)->type & (CMD | ARGS))
			get_cmd_path(cmd->opts[0], cmd->path, *var_list);
		if (((t_token *)node->content)->type == DLESS)
			error = get_heredoc(node->next->content, "> ", *var_list);
		if (((t_token *)node->content)->type & (LESS | GREAT | DLESS | DGREAT)
			&& !cmd->ior_start)
			cmd->ior_start = node;
		node = node->next;
	}
	cmd->opts[i] = NULL;
	return (error || setup_file_redirections(cmd->ior_start, cmd, valid));
}

int	execute_cmd(t_list **var_list, t_cmd *cmd)
{
	if (!ft_strcmp(cmd->opts[0], "cd"))
		return (builtin_cd(cmd->opts, var_list));
	if (!ft_strcmp(cmd->opts[0], "echo"))
		return (builtin_echo(cmd->opts, var_list));
	if (!ft_strcmp(cmd->opts[0], "env"))
		return (builtin_env(cmd->opts, var_list));
	if (!ft_strcmp(cmd->opts[0], "exit"))
		return (builtin_exit(cmd->opts, var_list));
	if (!ft_strcmp(cmd->opts[0], "export"))
		return (builtin_export(cmd->opts, var_list));
	if (!ft_strcmp(cmd->opts[0], "pwd"))
		return (builtin_pwd(cmd->opts, var_list));
	if (!ft_strcmp(cmd->opts[0], "unset"))
		return (builtin_unset(cmd->opts, var_list));
	return (execve(cmd->path, cmd->opts, NULL));
}

int	exec(t_list *node, t_list **var_list, t_cmd *cmd, int valid)
{
	char	*s;

	if (cmd->fd[2])
		close(cmd->fd[2]);
	if (parse_expr(node, var_list, cmd, valid))
	{
		perror("heredoc error");
		exit(errno);
	}
	if ((cmd->fd[0] && dup2(cmd->fd[0], STDIN_FILENO) < 0)
		|| (cmd->fd[1] && dup2(cmd->fd[1], STDOUT_FILENO) < 0))
	{
		perror(cmd->opts[0]);
		exit(errno);
	}
	if (cmd->fd[0])
		close(cmd->fd[0]);
	if (cmd->fd[1])
		close(cmd->fd[1]);
	if (!valid || execute_cmd(var_list, cmd))
		exit(errno);
}

int	proto(t_ast *ast, t_list **var_list, t_cmd *cmd, int valid)
{
	int	status;

	if (((t_token *)ast->expr->content)->type & (SEMI | OR)
		&& ((t_token *)ast->left->expr->content)->type & (SEMI | OR))
		proto(ast->left, var_list, cmd, valid);
	if (((t_token *)ast->expr->content)->type == OR)
	{
		if (pipe(cmd->fd))
			return (1);
		ft_swap(&cmd->fd[0], &cmd->fd[2]);
	}
	if (((t_token *)ast->expr->content)->type & (SEMI | OR))
	{
		cmd->pid = fork();
		if (cmd->pid == 0)
			exec(ast->left->expr, var_list, cmd, valid);
		if (cmd->fd[1] && close(cmd->fd[1]))
			return (1);
		cmd->fd[1] = 0;
		if (((t_token *)ast->expr->content)->type == SEMI
			&& waitpid(cmd->pid, &status, 0) == -1)
			return (WEXITSTATUS(status));
		ast->expr = ast->right->expr;
	}
	return (0);
}

int	evaluate_expr(t_ast *ast, t_list **var_list, int valid)
{
	int		status;
	t_cmd	cmd;

	cmd = (t_cmd){0, {0, 0, 0}, "", NULL, NULL};
	proto(ast, var_list, &cmd, valid);
	if (((t_token *)ast->expr->content)->type
		& (CMD | LESS | DLESS | GREAT | DGREAT))
	{
		ft_swap(&cmd.fd[0], &cmd.fd[2]);
		cmd.pid = fork();
		if (cmd.pid == 0)
			exec(ast->expr, var_list, &cmd, valid);
		if (cmd.fd[0] && !close(cmd.fd[0]) && cmd.fd[2] && !close(cmd.fd[2]))
			return (1);
		waitpid(cmd.pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (0);
	}
}

int	interpret_ast(t_ast *ast, t_list **var_list, int valid)
{
	if (((t_token *)ast->expr->content)->type == AND_IF)
		return (!interpret_ast(ast->left, var_list, valid)
			&& !interpret_ast(ast->right, var_list, valid));
	if (((t_token *)ast->expr->content)->type == OR_IF)
		return (!interpret_ast(ast->left, var_list, valid)
			|| !interpret_ast(ast->right, var_list, valid));
	return (evaluate_expr(ast, var_list, valid));
}
