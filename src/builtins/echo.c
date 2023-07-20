/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 20:17:25 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/12 20:17:25 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	builtin_echo(t_list **token_list, t_list **var_list)
{
	int	type;
	int	flag;

	(void) var_list;
	flag = 0;
	*token_list = (*token_list)->next;
	type = ((t_token *)(*token_list)->content)->type;
	while (type < SPACES)
	{
		if (!ft_strncmp(((t_token *)(*token_list)->content)->data, "-n", 2))
			flag = 1;
		else
		{
			if (flag & 2)
				printf(" ");
			flag |= 2;
			printf("%s",((t_token *)(*token_list)->content)->data);
		}
		*token_list = (*token_list)->next;
		type = ((t_token *)(*token_list)->content)->type;
	}
	if (!(flag & 1))
		printf("\n");
	return (0);
}
