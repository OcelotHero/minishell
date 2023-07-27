/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 01:51:51 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/12 01:51:51 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "libft.h"
# include "types.h"
# include "utils.h"
# include "builtins.h"
# include "ft_fprintf.h"

# include <stdio.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>

void	ast_clear(t_ast **root, void (*del)(t_list *));
t_ast	*ast_new(t_ast *left, t_list *expr, t_ast *right);

int		consume_token(t_list **token_list, int type);
int		unit(t_list **token_list, t_ast **node);
int		factor(t_list **token_list, t_ast **node);
int		term(t_list **token_list, t_ast **node);
int		expr(t_list **token_list, t_ast **node);

int		execute_cmd(t_list **var_list, t_cmd *cmd);
int		evaluate_expr(t_ast *ast, t_list **var_list, int valid);
int		interpret_ast(t_ast *ast, t_list **var_list, int valid);

int		get_heredoc(t_token *token, char *prompt, t_list *var_list);
void	get_cmd_path(char *cmd, char *path, t_list *var_list);

#endif
