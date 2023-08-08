/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 02:56:31 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/07 02:56:31 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int narg, char **args, char **envs)
{
	int		ret;
	t_list	*vars;

	vars = NULL;
	if (setup_env_vars(&vars, envs))
		return (error_msg(errno, E_MLOC, strerror(errno)));
	if (narg == 3 && ft_strcmp(args[1], "-c"))
		ret = minishell_non_interactive(args, &vars, envs);
	else
		ret = minishell_interactive(&vars, envs);
	ft_lstclear(&vars, free);
	return (ret);
}
