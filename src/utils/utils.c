/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 15:58:08 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/19 15:58:08 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	g_errno;

char	*var_value(char *key, t_list *var_list)
{
	int		i;
	t_list	*node;

	node = var_list;
	while (node)
	{
		i = 0;
		while (((char *)node->content)[i] == *(key + i))
			i++;
		if (((char *)node->content)[i] == '=' &&
			!ft_isalnum(*(key + i)) && *(key + i) != '_')
			return (&((char *)node->content)[i + 1]);
		node = node->next;
	}
	return (NULL);
}

int	token_type(char *str)
{
	// if ((*str == '\t' || *str == ' '))
	// 	return (SPACES);
	if ((*str == '\t' || *str == '\n' || *str == ' '))
		return (SPACES);
	if (*str == '(')
		return (LPAREN);
	if (*str == ')')
		return (RPAREN);
	if (*str == ';')
		return (SEMI);
	if (*str == '<')
		return (LESS + (*(str + 1) == '<') * (DLESS - LESS));
	if (*str == '>')
		return (GREAT + (*(str + 1) == '>') * (DGREAT - GREAT));
	if (*str == '|')
		return (OR + (*(str + 1) == '|') * (OR_IF - OR));
	if (*str == '&')
		return (AND + (*(str + 1) == '&') * (AND_IF - AND));
	if (*str == '\0')
		return (END);
	return (WORD);
}

char	*token_str(int type)
{
	if (type == LPAREN)
		return ("(");
	if (type == RPAREN)
		return (")");
	if (type == SEMI)
		return (";");
	if (type == LESS)
		return ("<");
	if (type == DLESS)
		return ("<<");
	if (type == GREAT)
		return (">");
	if (type == DGREAT)
		return (">>");
	if (type == OR)
		return ("|");
	if (type == OR_IF)
		return ("||");
	if (type == AND)
		return ("&");
	if (type == AND_IF)
		return ("&&");
	return ("");
}

int	is_builtin(char *cmd)
{
	return (cmd && (!ft_strccmp(cmd, "cd", !LINUX)
			|| !ft_strccmp(cmd, "echo", !LINUX) || !ft_strccmp(cmd, "unset", 0)
			|| !ft_strccmp(cmd, "env", !LINUX) || !ft_strccmp(cmd, "export", 0)
			|| !ft_strccmp(cmd, "pwd", !LINUX) || !ft_strccmp(cmd, "exit", 0)));
}

int	error_msg(int error, char *format, ...)
{
	va_list	args;

	g_errno = error;
	va_start(args, format);
	ft_vdprintf(2, format, args);
	va_end(args);
	return (error != 0);
}
