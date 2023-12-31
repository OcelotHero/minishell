/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snagulap <snagulap@student.42heilbronn.	+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 04:07:08 by snagulap          #+#    #+#             */
/*   Updated: 2023/07/13 04:07:08 by snagulap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	comp_var(const char *s1, const char *s2)
{
	return (ft_strncmp(s1, s2, ft_strlen(s2) + 1) != '=');
}

/**
 * 
 * @param	opts		options -- UNSET deletes env variable from env list
 * @param	vars		Shell environment variables
 * @return 				returns error number.
 */
int	builtin_unset(char **opts, t_list **vars)
{
	int	i;
	int	j;
	int	error;

	i = 0;
	error = 0;
	while (opts[++i])
	{
		j = 0;
		error = 0;
		if ((ft_isalpha(opts[i][j]) || opts[i][j] == '_') && ++j)
			while (opts[i][j] && (ft_isalnum(opts[i][j]) || opts[i][j] == '_'))
				j++;
		if (opts[i][j] || !j)
			error = error_msg(1, E_UNST, opts[i]);
		if (!error && !opts[i][j])
			ft_lstremove_if(vars, opts[i], comp_var, free);
	}
	return (error);
}
