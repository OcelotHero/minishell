/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 03:09:13 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/13 03:09:13 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "libft.h"
# include "types.h"
# include "parser.h"
# include "ft_dprintf.h"
# include "parser.h"
<<<<<<< HEAD

# include <termios.h>
=======
>>>>>>> 97468d3b508126543b10eec73f7b4f912ed7f22f

char	*token_str(int type);
int		token_type(char *str);
int		is_builtin(char *cmd);
int		error_msg(int error, char *format, ...);
<<<<<<< HEAD
char	*var_value(char *key, t_list *var_list);

void	clear(void *content);
char	*prompt_line(char *prompt);
void	setup_termios(struct termios *termios);
char	*semi_line_iterator(char *line);
int		semi_syntax_handler(char *line);
=======
char	*prompt_line(char *prompt);
>>>>>>> 97468d3b508126543b10eec73f7b4f912ed7f22f

#endif
