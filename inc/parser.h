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

# include "builtins.h"
# include "interpret.h"
# include "get_next_line.h"
# include "utils.h"

# include <stdio.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <readline/history.h>
# include <readline/readline.h>

void	ast_clear(t_ast **root, void (*del)(void *));
t_ast	*ast_new(t_ast *left, t_list *expr, t_ast *right);

int		consume_token(t_list **tokens, int type);
int		unit(t_list **tokens, t_ast **node);
int		factor(t_list **tokens, t_ast **node);
int		term(t_list **tokens, t_ast **node);
int		expr(t_list **tokens, t_ast **node);

int		get_heredoc(t_token *token, char *prompt, t_list *vars);
void	get_cmd_path(char *cmd, char *path, t_list *vars);

#endif
