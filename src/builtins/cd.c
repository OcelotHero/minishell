/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snagulap <snagulap@student.42heilbronn. 	+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 03:40:27 by snagulap          #+#    #+#             */
/*   Updated: 2023/07/13 03:40:27 by snagulap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * 
 * 
 * @param	opts	options -- cd ~, cd - ..
 * @param	vars	Shell environment variables
 * @return 			returns error number if fails, otherwise returns 0 
 */
int	cd_arg_handler(char **opts, t_list **vars)
{
	if (opts[1] && opts[1][0] == '-' && !opts[1][1])
	{
		opts[1] = var_value("OLDPWD", *vars);
		if (!opts[1])
			return (error_msg(1, E_CDNS, "OLDPWD"));
		ft_dprintf(1, "%s\n", opts[1]);
	}
	if ((!opts[1] || !ft_strcmp(opts[1], "--")) && var_value("HOME", *vars)
		&& chdir(var_value("HOME", *vars)))
		return (error_msg(1, E_CHDR, var_value("HOME", *vars),
				strerror(errno)));
	if (opts[1] && ft_strcmp(opts[1], "--") && chdir(opts[1]))
		return (error_msg(1, E_CHDR, opts[1], strerror(errno)));
	if ((!opts[1] || !ft_strcmp(opts[1], "--")) && !var_value("HOME", *vars))
		return (error_msg(1, E_CDNS, "HOME"));
	return (0);
}

/**
 * 
 * @param	opts		options -- cd ~, cd - ..
 * @param	vars		Shell environment variables
 * @return 				returns error number if fails, otherwise returns 0 
 */
int	builtin_cd(char **opts, t_list **vars)
{
	char	*pwd[2];
	char	buf[BUFSIZ];

	if (cd_arg_handler(opts, vars))
		return (1);
	pwd[0] = ft_strjoin("OLDPWD=", var_value("PWD", *vars));
	pwd[1] = ft_strjoin("PWD=", getcwd(buf, BUFSIZ));
	if (!pwd[0] || !pwd[1])
	{
		if (pwd[0])
			free(pwd[0]);
		if (pwd[1])
			free(pwd[1]);
		return (error_msg(errno, E_MLOC, strerror));
	}
	if (builtin_export((char *[]){"export", pwd[0], pwd[1], NULL}, vars))
		return (1);
	return (0);
}
