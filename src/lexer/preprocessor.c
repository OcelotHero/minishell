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

static int	is_escaped(int *i, char *wrd, int state, int type)
{
	int	res;

	res = ((wrd[*i] == '\\' && wrd[*i + 1] != '\\' && state == DQUOTE)
			|| (wrd[*i] == '\'' && state == DQUOTE)
			|| ((wrd[*i] == '"' || wrd[*i] == '\\') && state == SQUOTE)
			|| (wrd[*i] == '\\' && wrd[*i + 1] == '$' && state == DEFAULT));
	*i += ((state == DQUOTE && wrd[*i] == '\\'
				&& (wrd[*i + 1] == '"' || wrd[*i + 1] == '$'))
			|| (state == DEFAULT && wrd[*i] == '\\' && wrd[*i + 1] == '$'));
	return (res);
}

int	data_length(char *wrd, int *n, t_list *vars)
{
	int	i;
	int	count;
	int	state;

	i = -1;
	count = 0;
	state = DEFAULT;
	while (wrd[++i] && i < n[0])
	{
		if (state != SQUOTE && wrd[i] == '$' && n[1] != DLESS)
			count += interpolation_length(&i, wrd, vars);
		else
		{
			if (is_escaped(&i, wrd, state, n[1]))
				count++;
			else if ((state == SQUOTE && wrd[i] == '\'')
				|| (state == DQUOTE && wrd[i] == '"'))
				state = DEFAULT;
			else if (state == DEFAULT && (wrd[i] == '\'' || wrd[i] == '"'))
				state = (wrd[i] != '"') * SQUOTE + (wrd[i] == '"') * DQUOTE;
			count++;
		}
	}
	return (count);
}

int	populate_data(char *wrd, int *n, char *data, t_list *vars)
{
	int	i;
	int	*s;

	i = -1;
	s = (int []){DEFAULT, WORD};
	while (wrd[++i] && i < n[0])
	{
		if (s[0] != SQUOTE && wrd[i] == '*')
			s[1] |= WILD;
		if (s[0] != SQUOTE && wrd[i] == '$' && n[1] != DLESS)
			interpolate_var(&i, wrd, vars, &data);
		else
		{
			if (is_escaped(&i, wrd, s[0], n[1]))
				*(data++) = '\\';
			else if ((s[0] == SQUOTE && wrd[i] == '\'')
				|| (s[0] == DQUOTE && wrd[i] == '"'))
				s[0] = DEFAULT;
			else if (s[0] == DEFAULT && (wrd[i] == '\'' || wrd[i] == '"'))
				s[0] = (wrd[i] != '"') * SQUOTE + (wrd[i] == '"') * DQUOTE;
			*(data++) = wrd[i];
		}
	}
	*data = '\0';
	return (s[1]);
}
