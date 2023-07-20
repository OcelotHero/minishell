/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 00:29:55 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/14 00:29:55 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	ast_clear(t_ast **root, void (*del)(t_list *))
{
	if (!root || !*root)
		return ;
	ast_clear(&((*root)->left), del);
	ast_clear(&((*root)->right), del);
	if (del)
		(*del)((*root)->expr);
	free(*root);
	*root = NULL;
}

t_ast	*ast_new(t_ast *left, t_list *expr, t_ast *right)
{
	t_ast	*node;

	node = malloc(sizeof(*node));
	if (!node)
	{
		printf("alloc error\n");
		ast_clear(&left, NULL);
		ast_clear(&right, NULL);
		return (NULL);
	}
	node->expr = expr;
	node->left = left;
	node->right = right;
	return (node);
}
