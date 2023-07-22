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

static int	escaped_length(int *i, char *wrd, int *s)
{
	int	count;

	count = 1;
	if ((s[0] == DQUOTE && wrd[*i] != '\'' && wrd[*i + 1] != '"'
			&& wrd[*i + 1] != '\\' && wrd[*i + 1] != '$')
			|| (s[1] == DLESS && wrd[*i] == '$'))
		count++;
	*i += s[0] != SQUOTE && (s[0] != DQUOTE || wrd[*i] != '\'');
	return (count);
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
		if (wrd[i] == '\\' || (wrd[i] == '\'' && state == DQUOTE)
			|| (wrd[i] == '"' && state == SQUOTE)
			|| (n[1] == DLESS && wrd[i] == '$'))
			count += escaped_length(&i, wrd, (int []){state, n[1]});
		else if ((wrd[i] == '\'' && state == SQUOTE)
			|| (wrd[i] == '"' && state == DQUOTE))
			state = DEFAULT;
		else if ((wrd[i] == '\'' || wrd[i] == '"') && state == DEFAULT)
			state = (wrd[i] != '"') * SQUOTE + (wrd[i] == '"') * DQUOTE;
		if (state != SQUOTE && wrd[i] == '$' && (!i || wrd[i - 1] != '\\'))
			count += interpolation_length(&i, wrd, vars);
		else
			count++;
	}
	return (count);
}

static void	escaped_data(int *i, char *wrd, int *s, char **data)
{
	*((*data)++) = '\\';
	if ((s[0] == DQUOTE && wrd[*i] != '\'' && wrd[*i + 1] != '"'
			&& wrd[*i + 1] != '\\' && wrd[*i + 1] != '$')
			|| (s[1] == DLESS && wrd[*i] == '$'))
		*((*data)++) = wrd[*i];
	*i += s[0] != SQUOTE && (s[0] != DQUOTE || wrd[*i] != '\'');
}

int	populate_data(char *wrd, int *n, char *data, t_list *vars)
{
	int	i;
	int	*s;

	i = -1;
	s = (int []){DEFAULT, WORD};
	while (wrd[++i] && i < n[0])
	{
		if (wrd[i] == '\\' || (wrd[i] == '\'' && s[0] == DQUOTE)
			|| (wrd[i] == '"' && s[0] == SQUOTE)
			|| n[1] == DLESS && wrd[i] == '$')
			escaped_data(&i, wrd, (int []){s[0], n[1]}, &data);
		else if ((wrd[i] == '\'' && s[0] == SQUOTE)
			|| (wrd[i] == '"' && s[0] == DQUOTE))
			s[0] = DEFAULT;
		else if ((wrd[i] == '\'' || wrd[i] == '"') && s[0] == DEFAULT)
			s[0] = (wrd[i] != '"') * SQUOTE + (wrd[i] == '"') * DQUOTE;
		if (s[0] != SQUOTE && wrd[i] == '*' && (!i || wrd[i - 1] != '\\'))
			s[1] = (wrd[i] == '*') * WILD;
		if (s[0] != SQUOTE && wrd[i] == '$' && (!i || wrd[i - 1] != '\\'))
			interpolate_var(&i, wrd, vars, &data);
		else
			*(data++) = wrd[i];
	}
	*data = '\0';
	return (s[1]);
}
