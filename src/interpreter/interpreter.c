/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snagulap <snagulap@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:10:27 by snagulap          #+#    #+#             */
/*   Updated: 2023/08/07 14:23:15 by snagulap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpret.h"
#include <sys/stat.h>

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
		if (exec_child(ast->left->expr, vars, cmd, valid))
			return (1);
		ast->expr = NULL;
		if (ast->right)
			ast->expr = ast->right->expr;
	}
	return (0);
}

int	interpret_ast(t_ast *ast, t_list **vars, t_cmd *cmd, int valid)
{
	if (ast && ((t_token *)ast->expr->content)->type == AND_IF)
		return ((interpret_ast(ast->left, vars, cmd, valid))
			|| interpret_ast(ast->right, vars, cmd, valid));
	if (ast && ((t_token *)ast->expr->content)->type == OR_IF)
		return ((interpret_ast(ast->left, vars, cmd, valid) || !valid)
			&& interpret_ast(ast->right, vars, cmd, valid));
	if (ast && ast->expr && evaluate_expr(ast, vars, cmd, valid))
		return (1);
	if (ast && cmd->pid && ast->expr)
		if (exec_child(ast->expr, vars, cmd, 2 | valid))
			return (1);
	return (0);
}
