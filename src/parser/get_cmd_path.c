/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snagulap <snagulap@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 18:22:23 by rraharja          #+#    #+#             */
/*   Updated: 2023/08/08 11:51:22 by snagulap         ###   ########.fr       */
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

	paths = var_value("PATH", vars);
	if ((!paths || !*paths) && !access(cmd, F_OK))
	{
		ft_memcpy(path, cmd, ft_strlen(cmd) + 1);
		return ;
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
		if (!access(path, F_OK))
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
	if (cmd[0] == '.' && (!cmd[1] || (cmd[1] == '.' && !cmd[2])))
		return ;
	i = 0;
	while (cmd[i] && cmd[i] != '/')
		i++;
	if (cmd[i] == '/')
	{
		if (!access(cmd, F_OK))
			ft_memcpy(path, cmd, ft_strlen(cmd) + 1);
		return ;
	}
	get_env_cmd_path(cmd, path, vars);
}
