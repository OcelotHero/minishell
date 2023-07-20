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

int	consume_token(t_list **token_list, int type)
{
	int	state;

	if (type & (LPAREN | OR | SEMI | OR_IF | AND_IF))
	{
		*token_list = (*token_list)->next;
		return (0);
	}
	state = (type >= SPACES);
	*token_list = (*token_list)->next;
	type = ((t_token *)(*token_list)->content)->type;
	while (!(type & (LPAREN | RPAREN | OR | SEMI | OR_IF | AND_IF | END)))
	{
		if (state && (type >= SPACES))
			return (2);
		state = type >= SPACES;
		*token_list = (*token_list)->next;
		type = ((t_token *)(*token_list)->content)->type;
	}
	if (state && (type >= SPACES))
		return (2);
	return (0);
}

int	unit(t_list **token_list, t_ast **node)
{
	int		type;
	int		error;
	t_list	*token;

	token = *token_list;
	type = ((t_token *)token->content)->type;
	if (type & (CMD | WILD | LESS | GREAT | DLESS | DGREAT))
	{
		error = consume_token(token_list, type);
		*node = ast_new(NULL, token, NULL);
		return (error + !error * !(*node));
	}
	return (2);
}

int	factor(t_list **token_list, t_ast **node)
{
	int		error;
	t_list	*token;

	error = unit(token_list, node);
	if (error)
		return (error);
	while (((t_token *)(*token_list)->content)->type & (OR | SEMI))
	{
		token = *token_list;
		if (((t_token *)token->content)->type == OR)
			consume_token(token_list, OR);
		else
			consume_token(token_list, SEMI);
		*node = ast_new(*node, token, NULL);
		if (!(*node))
			return (1);
		error = unit(token_list, &((*node)->right));
		if (error)
			return (error);
	}
	return (0);
}

int	term(t_list **token_list, t_ast **node)
{
	int		type;
	int		error;
	t_list	*token;

	token = *token_list;
	type = ((t_token *)token->content)->type;
	if (type == LPAREN)
	{
		consume_token(token_list, LPAREN);
		error = expr(token_list, node);
		type = ((t_token *)(*token_list)->content)->type;
		if (error || type == END)
			return (error + !error * 2);
		consume_token(token_list, RPAREN);
		return (0);
	}
	return (factor(token_list, node));
}

int	expr(t_list **token_list, t_ast **node)
{
	int		error;
	t_list	*token;

	error = term(token_list, node);
	if (error)
		return (error);
	while (((t_token *)(*token_list)->content)->type & (OR_IF | AND_IF))
	{
		token = *token_list;
		if (((t_token *)token->content)->type == OR_IF)
			consume_token(token_list, OR_IF);
		else
			consume_token(token_list, AND_IF);
		*node = ast_new(*node, token, NULL);
		if (!(*node))
			return (1);
		error = term(token_list, &((*node)->right));
		if (error)
			return (error);
	}
	return (0);
}
