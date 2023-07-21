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

int	data_length(char *wrd, int n, t_list *vars)
{
	int	i;
	int	count;
	int	state;

	i = -1;
	count = 0;
	state = DEFAULT;
	while (wrd[++i] && i < n)
	{
		if (((wrd[i + 1] == '"' || wrd[i + 1] == '\\' || wrd[i + 1] == '$')
				&& state == DQUOTE || state == DEFAULT) && wrd[i] == '\\')
			i++;
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

int	populate_data(char *wrd, int n, char *data, t_list *vars)
{
	int	i;
	int	*s;

	i = -1;
	s = (int []){DEFAULT, WORD};
	while (wrd[++i] && i < n)
	{
		if (((wrd[i + 1] == '"' || wrd[i + 1] == '\\' || wrd[i + 1] == '$')
				&& s[0] == DQUOTE || s[0] == DEFAULT) && wrd[i] == '\\')
			i++;
		else if ((wrd[i] == '\'' && s[0] == SQUOTE)
			|| (wrd[i] == '"' && s[0] == DQUOTE))
			s[0] = DEFAULT;
		else if ((wrd[i] == '\'' || wrd[i] == '"') && s[0] == DEFAULT)
			s[0] = (wrd[i] != '"') * SQUOTE + (wrd[i] == '"') * DQUOTE;
		if (s[0] != SQUOTE && (wrd[i] == '*' || wrd[i] == '$')
			&& (!i || wrd[i - 1] != '\\'))
			s[1] = ((wrd[i] == '*') * WILD) | ((wrd[i] == '$') * VARS);
		if (s[0] != SQUOTE && wrd[i] == '$' && (!i || wrd[i - 1] != '\\'))
			interpolate_var(&i, wrd, vars, &data);
		else
			*(data++) = wrd[i];
	}
	*data = '\0';
	return (s[1]);
}