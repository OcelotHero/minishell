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
	if (type & (LESS | DLESS))
		return ((char []){'<', '<' * (type == DLESS), '\0'});
	if (type & (GREAT | DGREAT))
		return ((char []){'>', '>' * (type == DGREAT), '\0'});
	if (type & (OR | OR_IF))
		return ((char []){'|', '|' * (type == OR_IF), '\0'});
	if (type & (AND | AND_IF))
		return ((char []){'&', '&' * (type == AND_IF), '\0'});
	return ("");
}

int	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "unset"));
}

int	error_msg(int error, char *format, ...)
{
	va_list	args;

	g_errno = error;
	va_start(args, format);
	ft_vdprintf(2, format, args);
	va_end(args);
	return (1);
}
