/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 00:44:09 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/12 00:44:09 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	consume_token(t_list **tokens, int type)
{
	int	state;

	if (type & (LPAREN | OR | SEMI | OR_IF | AND_IF))
	{
		*tokens = (*tokens)->next;
		return (0);
	}
	state = (type >= SPACES);
	*tokens = (*tokens)->next;
	type = ((t_token *)(*tokens)->content)->type;
	while (!(type & (LPAREN | RPAREN | OR | SEMI | OR_IF | AND_IF | END)))
	{
		if (state && (type >= SPACES))
			return (2);
		state = type >= SPACES;
		*tokens = (*tokens)->next;
		type = ((t_token *)(*tokens)->content)->type;
	}
	if (state && (type >= SPACES))
		return (2);
	return (0);
}

int	unit(t_list **tokens, t_ast **node)
{
	int		type;
	int		error;
	t_list	*token;

	token = *tokens;
	type = ((t_token *)token->content)->type;
	if (type & (CMD | WILD | LESS | GREAT | DLESS | DGREAT))
	{
		error = consume_token(tokens, type);
		*node = ast_new(NULL, token, NULL);
		return (error + !error * !(*node));
	}
	return (2);
}

int	factor(t_list **tokens, t_ast **node)
{
	int		error;
	t_list	*token;

	error = unit(tokens, node);
	if (error)
		return (error);
	while (((t_token *)(*tokens)->content)->type & (OR | SEMI))
	{
		token = *tokens;
		if (((t_token *)token->content)->type == OR)
			consume_token(tokens, OR);
		else
			consume_token(tokens, SEMI);
		*node = ast_new(*node, token, NULL);
		if (!(*node))
			return (1);
		error = unit(tokens, &((*node)->right));
		if (error)
			return (error);
	}
	return (0);
}

int	term(t_list **tokens, t_ast **node)
{
	int		type;
	int		error;
	t_list	*token;

	token = *tokens;
	type = ((t_token *)token->content)->type;
	if (type == LPAREN)
	{
		consume_token(tokens, LPAREN);
		error = expr(tokens, node);
		type = ((t_token *)(*tokens)->content)->type;
		if (error || type == END)
			return (error + !error * 2);
		consume_token(tokens, RPAREN);
		return (0);
	}
	return (factor(tokens, node));
}

int	expr(t_list **tokens, t_ast **node)
{
	int		error;
	t_list	*token;

	error = term(tokens, node);
	if (error)
		return (error);
	while (((t_token *)(*tokens)->content)->type & (OR_IF | AND_IF))
	{
		token = *tokens;
		if (((t_token *)token->content)->type == OR_IF)
			consume_token(tokens, OR_IF);
		else
			consume_token(tokens, AND_IF);
		*node = ast_new(*node, token, NULL);
		if (!(*node))
			return (1);
		error = term(tokens, &((*node)->right));
		if (error)
			return (error);
	}
	return (0);
}
