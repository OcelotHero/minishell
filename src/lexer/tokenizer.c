/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 02:56:37 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/07 02:56:37 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	*refine_token(char *str, int *n, char *data, t_list *vars)
{
	t_token	*token;

	token = malloc(sizeof(*token));
	if (!token)
	{
		free(data);
		return (NULL);
	}
	token->type = populate_data(str, n, data, vars);
	if ((token->type & WILD) && !BONUS)
		token->type ^= WILD;
	if (token->type == WORD)
		token->type = token_type(str);
	token->data = data;
	if (token->type == WORD)
	{
		if (n[1] & (LESS | GREAT | DLESS | DGREAT))
			token->type |= FILES;
		else if (data[0] == '-' && (ft_isalnum(data[1])
				|| (data[1] == '-' && ft_isalpha(data[2]))))
			token->type |= OPTS1 + (data[1] == '-') * (OPTS2 - OPTS1);
		else
			token->type |= CMD * !n[2] + ARGS * n[2];
	}
	return (token);
}

t_list	*update_prev_token(t_list *prev, t_token *token, char **strs, int n)
{
	char	*data;
	t_list	*node;

	if (prev && ((t_token *)prev->content)->type & (LESS | GREAT | DGREAT))
	{
		data = ft_strndup(strs[1], n);
		if (!data)
		{
			free(strs[0]);
			free(token);
			return (NULL);
		}
		free(((t_token *)prev->content)->data);
		((t_token *)prev->content)->data = data;
	}
	node = ft_lstnew(token);
	if (!node)
	{
		free(strs[0]);
		free(token);
		return (NULL);
	}
	return (node);
}

int	save_token(t_list **tokens, char *str, int n, t_list *vars)
{
	static int		type = SPACES;
	static int		flag = 0;
	static t_list	*node = NULL;
	char			*data;
	t_token			*token;

	data = malloc(sizeof(*data)
			* (data_length(str, (int []){n, type}, vars) + 1));
	if (!data)
		return (1);
	token = refine_token(str, (int []){n, type, flag}, data, vars);
	if (!token)
		return (1);
	node = update_prev_token(node, token, (char *[]){data, str}, n);
	if (!node)
		return (1);
	ft_lstadd_back(tokens, node);
	type = token_type(str);
	flag |= (token->type == CMD && ft_strlen(data));
	flag &= !(type & (LPAREN | SEMI | OR | AND | OR_IF | AND_IF | END));
	if (type == END)
		node = NULL;
	return (0);
}

int	tokenize_line(t_list **tokens, int *n, char *str, t_list *vars)
{
	int	type;
	int	error;

	error = 0;
	type = token_type(&str[n[0]]);
	if (type >= SPACES)
	{
		if (type >= SPACES && n[1] != n[0])
			error = save_token(tokens, &str[n[1]], n[0] - n[1], vars);
		if (!error && type > SPACES)
			error = save_token(tokens, &str[n[0]], 1 + (type >= DLESS), vars);
		n[0] += (type >= DLESS);
		n[1] = n[0] + 1;
	}
	return (error);
}

int	tokenize(t_list **tokens, char *str, t_list *vars)
{
	int	*n;
	int	s;

	n = (int []){-1, 0, ft_strlen(str)};
	s = DEFAULT;
	while (++(n[0]) <= n[2])
	{
		if (s != SQUOTE && str[n[0]] == '\\' && !str[n[0] + 1])
			break ;
		if (s != SQUOTE && str[n[0]] == '\\')
			(n[0])++;
		else if ((str[n[0]] == '\'' && s == SQUOTE)
			|| (str[n[0]] == '"' && s == DQUOTE))
			s = DEFAULT;
		else if ((str[n[0]] == '\'' || str[n[0]] == '"') && s == DEFAULT)
			s = (str[n[0]] != '"') * SQUOTE + (str[n[0]] == '"') * DQUOTE;
		else if ((s == DEFAULT || !str[n[0]])
			&& tokenize_line(tokens, n, str, vars))
			return (1);
	}
	if (s != DEFAULT)
		error_msg(1, E_QUOT, (s == SQUOTE) * '\'' + (s != SQUOTE) * '"');
	else if (n[0] <= n[2])
		error_msg(1, E_ESCP);
	return (s != DEFAULT || n[0] <= n[2]);
}
