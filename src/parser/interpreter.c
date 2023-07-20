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

int	other(t_list **token_list, t_list **var_list)
{
	return (0);
}

int	execute_builtin(t_list **token_list, t_list **var_list)
{
	if (!ft_strcmp(((t_token *)(*token_list)->content)->data, "cd"))
		return (builtin_cd(token_list, var_list));
	if (!ft_strcmp(((t_token *)(*token_list)->content)->data, "echo"))
		return (builtin_echo(token_list, var_list));
	if (!ft_strcmp(((t_token *)(*token_list)->content)->data, "env"))
		return (builtin_env(token_list, var_list));
	if (!ft_strcmp(((t_token *)(*token_list)->content)->data, "exit"))
		return (builtin_exit(token_list, var_list));
	if (!ft_strcmp(((t_token *)(*token_list)->content)->data, "export"))
		return (builtin_export(token_list, var_list));
	if (!ft_strcmp(((t_token *)(*token_list)->content)->data, "pwd"))
		return (builtin_pwd(token_list, var_list));
	if (!ft_strcmp(((t_token *)(*token_list)->content)->data, "unset"))
		return (builtin_unset(token_list, var_list));
	return (other(token_list, var_list));
}

int	setup_file_redirections(t_list *node, int *fd)
{
	int		io;
	int		type;
	int		flags;
	char	*file;

	type = ((t_token *)node->content)->type;
	while (!(type & (LPAREN | RPAREN | OR | SEMI | OR_IF | AND_IF | END)))
	{
		if (type & (LESS | GREAT | DLESS | DGREAT))
		{
			io = !(type & (LESS | DLESS));
			flags = io * (O_WRONLY | O_CREAT | O_TRUNC * (type == GREAT)
					| O_APPEND * (type == DGREAT)) + !io * O_RDONLY;
			file = ((t_token *)node->next->content)->data;
			if (type == DLESS)
				file = ".tmp";
			if (!fd[io] || !close(fd[io]))
				fd[io] = open(file, flags, 0644);
			if (fd[io] < 0)
				return (1);
		}
		node = node->next;
		type = ((t_token *)node->content)->type;
	}
	return (0);
}

int	parse_expr(t_ast *ast, t_list **var_list, t_cmd *cmd)
{
	int		i;
	t_list	*node;

	i = 0;
	node = ast->expr;
	while (!(((t_token *)node->content)->type
			& (LPAREN | RPAREN | OR | SEMI | OR_IF | AND_IF | END)))
	{
		if (((t_token *)node->content)->type & (CMD | OPTS1 | OPTS2 | ARGS))
			cmd->opts[i++] = ((t_token *)node->content)->data;
		if (((t_token *)node->content)->type == CMD)
			get_cmd_path(cmd->opts[0], cmd->path, *var_list);
		if (((t_token *)node->content)->type == DLESS
			&& get_heredoc(((t_token *)node->next->content)->data, ">"))
			return (1);
		if (((t_token *)node->content)->type & (LESS | GREAT | DLESS | DGREAT)
			&& !cmd->ior_start)
			cmd->ior_start = node;
		node = node->next;
	}
	cmd->opts[i] = NULL;
	if (cmd->ior_start)
		return (setup_file_redirections(cmd->ior_start, cmd->fd));
	return (0);
}

int	proto(t_ast *ast, t_list **var_list, t_cmd *cmd)
{
	if (((t_token *)ast->expr->content)->type & (SEMI | OR)
		&& ((t_token *)ast->left->expr->content)->type & (SEMI | OR))
		proto(ast->left, var_list, cmd);
	if (((t_token *)ast->expr->content)->type & (SEMI | OR))
	{
		// printf("Here\n");
		// pipe(cmd->fd);
		// cmd->pid = fork();
		// // if (cmd.pid == -1)
		// 	// error
		// if (cmd->pid == 0)
			parse_expr(ast->left, var_list, cmd);
		ast->expr = ast->right->expr;
	}
	return (0);
}

int	evaluate_expr(t_ast *ast, t_list **var_list)
{
	t_cmd	cmd;

	cmd = (t_cmd){0, {0, 0}, "", NULL, NULL};
	proto(ast, var_list, &cmd);
	if (((t_token *)ast->expr->content)->type
		& (CMD | LESS | GREAT | DLESS | DGREAT))
	{
		// cmd.pid = fork();
		// // if (cmd.pid == -1)
		// 	// error
		// if (cmd.pid == 0)
			parse_expr(ast, var_list, &cmd);
		return (0);
	}
	return (0);
}

int	interpret_ast(t_ast *ast, t_list **var_list)
{
	if (((t_token *)ast->expr->content)->type == AND_IF)
		return (!interpret_ast(ast->left, var_list)
			&& !interpret_ast(ast->right, var_list));
	if (((t_token *)ast->expr->content)->type == OR_IF)
		return (!interpret_ast(ast->left, var_list)
			|| !interpret_ast(ast->right, var_list));
	return (evaluate_expr(ast, var_list));
}
