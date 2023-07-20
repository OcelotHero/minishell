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

# include <errno.h>
# include <stdio.h>
# include <unistd.h>

int	builtin_cd(t_list **token_list, t_list **var_list);
int	builtin_echo(t_list **token_list, t_list **var_list);
int	builtin_env(t_list **token_list, t_list **var_list);
int	builtin_exit(t_list **token_list, t_list **var_list);
int	builtin_export(t_list **token_list, t_list **var_list);
int	builtin_pwd(t_list **token_list, t_list **var_list);
int	builtin_unset(t_list **token_list, t_list **var_list);

#endif
