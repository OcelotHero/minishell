/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 21:06:15 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/09 21:06:15 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "utils.h"

# include <glob.h>
# include <stdio.h>
# include <errno.h>
# include <string.h>

int		tokenize(t_list **tokens, char *str, t_list *vars);
int		tokenize_line(t_list **tokens, int *n, char *str, t_list *vars);
int		save_token(t_list **tokens, char *str, int n, t_list *vars);
t_list	*update_prev_token(t_list *prev, t_token *token, char **str, int n);
t_token	*refine_token(char *str, int *n, char *data, t_list *vars);

int		postprocess(t_list *tokens);
int		postprocess_token(t_list **tokens, int *n, char *wrd);
int		split_token(t_list **tokens, char *wrd, int n);
int		append_token(t_list **tokens, t_list *node, char *data, int quot);
char	*unquote(char *wrd, int n, int *quot, int glob);

int		trim(t_list **tokens);
int		trim_blank_tokens(t_list **tokens);
int		trim_parenthesis_token(t_list **tokens, int ptype);

int		data_length(char *wrd, int *n, t_list *vars);
int		populate_data(char *wrd, int *n, char *data, t_list *vars);

int		interpolate_var(int **i, char *str, t_list *var_list, char **data);
int		interpolation_length(int **i, char *str, t_list *var_list);
int		interpolation_value(int **i, char *str, t_list *var_list, char **data);

int		expand_wildcard(t_list *token);
int		insert_matches(t_list *tokens, glob_t *gstruct, int n, int type);

#endif
