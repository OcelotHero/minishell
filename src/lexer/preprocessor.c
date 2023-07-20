/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocessor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 02:00:42 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/10 02:00:42 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	data_length(char *wrd, t_list *var_list)
{
	int	i;
	int	count;
	int	*s;

	i = -1;
	count = 0;
	s = (int []){DEFAULT, DEFAULT};
	while (wrd[++i])
	{
		if (((wrd[i + 1] == '"' || wrd[i + 1] == '\\' || wrd[i + 1] == '$')
				&& s[0] == DQUOTE || s[0] == DEFAULT) && wrd[i] == '\\')
			i++;
		else if ((wrd[i] == '\'' && s[0] == SQUOTE)
			|| (wrd[i] == '"' && s[0] == DQUOTE))
			s[0] = DEFAULT;
		else if ((wrd[i] == '\'' || wrd[i] == '"') && s[0] == DEFAULT)
			s[0] = (wrd[i] != '"') * SQUOTE + (wrd[i] == '"') * DQUOTE;
		if (s[0] != SQUOTE && wrd[i] == '$' && (!i || wrd[i - 1] != '\\'))
			count += interpolation_length(&i, wrd, var_list);
		else if (s[0] == s[1])
			count++;
		s[1] = s[0];
	}
	return (count);
}

int	populate_data(char *wrd, t_list *var_list, char *data)
{
	int	*i;
	int	*s;

	i = (int []){-1, 0};
	s = (int []){DEFAULT, DEFAULT};
	while (wrd[++(*i)])
	{
		if (((wrd[*i + 1] == '"' || wrd[*i + 1] == '\\' || wrd[*i + 1] == '$')
				&& s[0] == DQUOTE || s[0] == DEFAULT) && wrd[*i] == '\\')
			(*i)++;
		else if ((wrd[*i] == '\'' && s[0] == SQUOTE)
			|| (wrd[*i] == '"' && s[0] == DQUOTE))
			s[0] = DEFAULT;
		else if ((wrd[*i] == '\'' || wrd[*i] == '"') && s[0] == DEFAULT)
			s[0] = (wrd[*i] != '"') * SQUOTE + (wrd[*i] == '"') * DQUOTE;
		if (s[0] != SQUOTE && wrd[*i] == '$' && (!(*i) || wrd[*i - 1] != '\\'))
			interpolate_var(i, wrd, var_list, &data);
		else if (s[0] == s[1])
			*(data++) = wrd[*i];
		if (s[0] != SQUOTE && wrd[*i] == '*' && (!(*i) || wrd[*i - 1] != '\\'))
			i[1] = 1;
		s[1] = s[0];
	}
	*data = '\0';
	return (i[1]);
}

void	refine_token(t_token *token, t_list *var_list, char *data, int type)
{
	if (populate_data(token->data, var_list, data))
		token->type = WILD * BONUS;
	free(token->data);
	token->data = data;
	if (token->type != WILD)
	{
		if (type & (LESS | GREAT | DLESS | DGREAT))
			token->type = FILES;
		else
			token->type = CMD * (type >= SPACES) + ARGS * (type < SPACES);
		if (data[0] == '-' && (ft_isalnum(data[1])
				|| (data[1] == '-' && ft_isalpha(data[2]))))
			token->type = OPTS1 + (data[1] == '-') * (OPTS2 - OPTS1);
	}
	// else if ()
}

int	preprocess_token(t_list *token_list, t_list *var_list)
{
	int		type;
	char	*data;
	t_list	*node;
	t_token	*token;

	node = token_list;
	type = SPACES;
	while (node)
	{
		token = (t_token *)node->content;
		if (token->type == WORD)
		{
			data = malloc(sizeof(*data)
					* (data_length(token->data, var_list) + 1));
			if (!data)
				return (1);
			refine_token(token, var_list, data, type);
		}
		type = token->type;
		node = node->next;
	}
	return (0);
}
