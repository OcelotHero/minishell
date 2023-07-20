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

# include "libft.h"
# include "types.h"

# include <stdio.h>
# include <errno.h>

int		token_type(int i, char *str);
int		save_token(t_list **token_list, int start, int n, char *str);
int		tokenize_line(t_list **token_list, int *i, int *start, char *str);
int		tokenize(t_list **token_list, char *str);

int		data_length(char *wrd, t_list *var_list);
int		populate_data(char *wd, t_list *var_list, char *data);
void	refine_token(t_token *token, t_list *var_list, char *data, int type);
int		preprocess_token(t_list *token_list, t_list *var_list);

int		interpolation_length(int *i, char *str, t_list *var_list);
void	interpolation_value(int *i, char *str, t_list *var_list, char **data);
void	interpolate_var(int *i, char *str, t_list *var_list, char **data);

#endif
