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

int	resulting_length(char *line, t_list *vars, int *count)
{
	int	i;
	int	state;

	i = -1;
	*count = 0;
	state = DEFAULT;
	while (line[++i])
	{
		if (line[i] == '\\' && state != SQUOTE)
			i++;
		else if ((line[i] == '\'' && state == SQUOTE)
			|| (line[i] == '"' && state == DQUOTE))
			state = DEFAULT;
		else if ((line[i] == '\'' || line[i] == '"') && state == DEFAULT)
			state = (line[i] != '"') * SQUOTE + (line[i] == '"') * DQUOTE;
		if (state != SQUOTE && line[i] == '$' && (!i || line[i - 1] != '\\'))
			*count += interpolation_length(&i, line, vars);
		else
			*count += 1 + ((line[i] == '\'' || line[i] == '"')
				|| (i && line[i - 1] == '\\'));
	}
	return (state);
}

void	populate_result(char *line, t_list *vars, char *res)
{
	int	i;
	int	state;

	i = -1;
	state = DEFAULT;
	while (line[++i])
	{
		if (line[i] == '\\' && state != SQUOTE)
			*(res++) = line[i++];
		else if ((line[i] == '\'' && state == SQUOTE)
			|| (line[i] == '"' && state == DQUOTE))
			state = DEFAULT;
		else if ((line[i] == '\'' || line[i] == '"') && state == DEFAULT)
			state = (line[i] != '"') * SQUOTE + (line[i] == '"') * DQUOTE;
		if (state != SQUOTE && line[i] == '$' && (!i || line[i - 1] != '\\'))
			interpolate_var(&i, line, vars, &res);
		else
			*(res++) = line[i];
	}
	*res = '\0';
}

char	*preprocess_line(char *line, t_list *vars)
{
	int		n;
	int		state;
	char	*res;

	state = resulting_length(line, vars, &n);
	if (state != DEFAULT)
	{
		printf("unexpected EOF while looking for matching %c\n",
			(state == SQUOTE) * '\'' + (state != SQUOTE) * '"');
		return (NULL);
	}
	res = malloc(sizeof(*res) * (n + 1));
	if (!res)
		return (NULL);
	populate_result(line, vars, res);
	// printf("%5d, %s\n", n, res);
	return (res);
}
