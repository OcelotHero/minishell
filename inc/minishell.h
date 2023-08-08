/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 21:06:15 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/09 21:06:15 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "utils.h"
# include "lexer.h"
# include "parser.h"
# include "signals.h"
# include "ft_dprintf.h"
# include "get_next_line.h"

# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>

int	setup_env_vars(t_list **vars, char **envs);
int	minishell_interactive(t_list **vars, char **envs);
int	minishell_non_interactive(char **args, t_list **vars, char **envs);

#endif