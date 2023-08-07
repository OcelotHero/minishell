/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snagulap <snagulap@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:32:46 by snagulap          #+#    #+#             */
/*   Updated: 2023/08/07 14:10:46 by snagulap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERPRET_H
# define INTERPRET_H

# include "builtins.h"
# include "parser.h"

# include <stdio.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <readline/history.h>
# include <readline/readline.h>

int	exec_child(t_list *node, t_list **vars, t_cmd *cmd, int flags);
int	evaluate_expr(t_ast *ast, t_list **vars, t_cmd *cmd, int valid);
int	interpret_ast(t_ast *ast, t_list **vars, t_cmd *cmd, int valid);
int	setup_file_fds(t_list **node, t_cmd *cmd, int type);
int	setup_file_redirections(t_list *node, t_cmd *cmd);
int	parse_expr(t_list *node, t_list **vars, t_cmd *cmd);
int	exec_builtin(t_list **vars, t_cmd *cmd);
int	execute_cmd(t_list **vars, t_cmd *cmd);
int	exec(t_list **vars, t_cmd *cmd);
#endif