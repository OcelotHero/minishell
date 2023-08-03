/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 18:22:23 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/17 18:18:24 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/**
 * Returns the full path to the command invoked, using the path environment
 * variable if available, NULL otherwise
 */
static void	get_env_cmd_path(char *cmd, char *path, t_list *vars)
{
	char	*sep;
	char	*paths;

	paths = NULL;
	while (vars && !paths)
	{
		if (!ft_strncmp(vars->content, "PATH=", 5))
			paths = ft_strchr(vars->content, '=') + 1;
		vars = vars->next;
	}
	sep = ":";
	while (*sep && paths)
	{
		sep = ft_strchr(paths, ':');
		if (!sep)
			sep = ft_strchr(paths, '\0');
		ft_memcpy(path, paths, sep - paths);
		path[sep - paths] = '/';
		ft_memcpy(path + (sep - paths) + 1, cmd, ft_strlen(cmd) + 1);
		if (access(path, F_OK) == 0)
			return ;
		paths = sep + 1;
	}
	path[0] = '\0';
}

/**
 * Returns the path to the command invoked, looking through the path environment
 * variable if the command invoked. Returns NULL if the command is not found in
 * the relative path or the path environment.
 */
void	get_cmd_path(char *cmd, char *path, t_list *vars)
{
	int	i;

	if (!ft_strlen(cmd))
		return ;
	i = 0;
	if (access(cmd, F_OK) == 0)
	{
		while (cmd[i] && cmd[i] != '/')
			i++;
		if (cmd[i] == '/')
			ft_memcpy(path, cmd, ft_strlen(cmd) + 1);
		return ;
	}
	get_env_cmd_path(cmd, path, vars);
}
