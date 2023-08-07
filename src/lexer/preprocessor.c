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

	res = (wrd[*i] == '\\'
			|| ((wrd[*i] == '"' || wrd[*i] == '*') && state == SQUOTE)
			|| ((wrd[*i] == '\'' || wrd[*i] == '*') && state == DQUOTE));
	*i += ((((wrd[*i + 1] == '"' || wrd[*i + 1] == '\\' || wrd[*i + 1] == '$')
					&& state == DQUOTE)
				|| state == DEFAULT) && wrd[*i] == '\\');
	return (res);
}

int	data_length(char *wrd, int *n, t_list *vars)
{
	int	i;
	int	count;
	int	s;

	i = -1;
	count = 0;
	s = DEFAULT;
	while (wrd[++i] && i < n[0])
	{
		if (n[1] != DLESS && ((s != SQUOTE && wrd[i] == '$') || (s == DEFAULT
					&& wrd[i] == '~' && (!wrd[i + 1] || wrd[i + 1] == '\\'))))
			count += interpolation_length((int *[]){&i, &s}, wrd, vars);
		else
		{
			if (is_escaped(&i, wrd, s, n[1]))
				count++;
			else if ((s == SQUOTE && wrd[i] == '\'')
				|| (s == DQUOTE && wrd[i] == '"'))
				s = DEFAULT;
			else if (s == DEFAULT && (wrd[i] == '\'' || wrd[i] == '"'))
				s = (wrd[i] != '"') * SQUOTE + (wrd[i] == '"') * DQUOTE;
			count++;
		}
	}
	return (count);
}

int	populate_data(char *wrd, int *n, char *data, t_list *vars)
{
	int	*s;

	s = (int []){-1, DEFAULT, WORD};
	while (wrd[++(*s)] && *s < n[0])
	{
		if (s[1] == DEFAULT && wrd[*s] == '*' && n[1] != DLESS)
			s[2] |= WILD;
		if (((!s[1] && wrd[*s] == '~' && (!wrd[*s + 1] || wrd[*s + 1] == '\\'))
				|| (s[1] != SQUOTE && wrd[*s] == '$')) && n[1] != DLESS)
			s[2] |= interpolate_var((int *[]){&(*s), &s[1]}, wrd, vars, &data);
		else
		{
			if (is_escaped(s, wrd, s[1], n[1]))
				*(data++) = '\\';
			else if ((s[1] == SQUOTE && wrd[*s] == '\'')
				|| (s[1] == DQUOTE && wrd[*s] == '"'))
				s[1] = DEFAULT;
			else if (s[1] == DEFAULT && (wrd[*s] == '\'' || wrd[*s] == '"'))
				s[1] = (wrd[*s] != '"') * SQUOTE + (wrd[*s] == '"') * DQUOTE;
			*(data++) = wrd[*s];
		}
	}
	*data = '\0';
	return (s[2]);
}
