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

# include <stdio.h>
# include <errno.h>

t_token	*refine_token(char *str, int *n, char *data, t_list *vars);
t_list	*update_prev_token(t_list *prev, t_token *token, char **str, int n);
int		save_token(t_list **tokens, char *str, int n, t_list *vars);
int		tokenize_line(t_list **tokens, int *n, char *str, t_list *vars);
int		tokenize(t_list **tokens, char *str, t_list *vars);

char	*unquote(char *wrd, int n, int *unquoted);
int		append_token(t_list **tokens, t_list *node, char *data, int unquoted);
int		split_token(t_list **tokens, char *wrd, int n);
int		postprocess(t_list **tokens, int *n, char *wrd);
int		postprocess_tokens(t_list *tokens);

void	trim_tokens(t_list **tokens);

int		data_length(char *wrd, int *n, t_list *vars);
int		populate_data(char *wrd, int *n, char *data, t_list *vars);

int		interpolation_length(int *i, char *str, t_list *var_list);
void	interpolation_value(int *i, char *str, t_list *var_list, char **data);
void	interpolate_var(int *i, char *str, t_list *var_list, char **data);

#endif
