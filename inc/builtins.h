/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 03:09:13 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/13 03:09:13 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "libft.h"
# include "types.h"
# include "ft_fprintf.h"

# include <errno.h>
# include <stdio.h>
# include <unistd.h>

int	builtin_cd(char **opts, t_list **var_list);
int	builtin_echo(char **opts, t_list **var_list);
int	builtin_env(char **opts, t_list **var_list);
int	builtin_exit(char **opts, t_list **var_list);
int	builtin_export(char **opts, t_list **var_list);
int	builtin_pwd(char **opts, t_list **var_list);
int	builtin_unset(char **opts, t_list **var_list);

#endif
