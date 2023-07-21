/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postprocessor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 02:00:42 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/10 02:00:42 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	postprocess_tokens(t_list *tokens)
{
	int	*n;
	int	state;

	while (tokens)
	{
		n = (int []){-1, 0, ft_strlen(str)};
		state = DEFAULT;
		while (++(n[0]) <= n[2])
		{
			if (state != SQUOTE && str[n[0]] == '\\')
				(n[0])++;
			else if ((str[n[0]] == '\'' && state == SQUOTE)
				|| (str[n[0]] == '"' && state == DQUOTE))
				state = DEFAULT;
			else if ((str[n[0]] == '\'' || str[n[0]] == '"') && state == DEFAULT)
				state = (str[n[0]] != '"') * SQUOTE + (str[n[0]] == '"') * DQUOTE;
			if (state == DEFAULT && tokenize_line(tokens, n, str, vars))
				return (1);
		}
		tokens
		while ()
	}
}