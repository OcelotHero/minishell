/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirector.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snagulap <snagulap@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:30:06 by snagulap          #+#    #+#             */
/*   Updated: 2023/08/07 14:10:14 by snagulap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpret.h"
#include <sys/stat.h>

int	setup_file_fds(t_list **node, t_cmd *cmd, int type)
{
	int	io;
	int	flags;

	io = !(type & (LESS | DLESS));
	flags = io * (O_WRONLY | O_CREAT | O_APPEND * ((type & DGREAT) != 0)
			| O_TRUNC * ((type & GREAT) != 0)) + !io * O_RDONLY;
	if (((t_token *)(*node)->next->content)->type == FILES
		&& ((t_token *)(*node)->next->next->content)->type == FILES)
		return (error_msg(1, E_AMBI, ((t_token *)(*node)->content)->data));
	*node = (*node)->next;
	if (type == DLESS && (!cmd->fd[io] || close(cmd->fd[io]) || 1))
		cmd->fd[io] = open(".tmp", flags, 0644);
	else if (!cmd->fd[io] || close(cmd->fd[io]) || 1)
		cmd->fd[io] = open(((t_token *)(*node)->content)->data,
				flags, 0644);
	if (cmd->fd[io] < 0)
		return (error_msg(1, E_FILE, ((t_token *)(*node)->content)->data,
			strerror(errno)));
	return (0);
}

int	setup_file_redirections(t_list *node, t_cmd *cmd)
{
	int	type;
	int	error;

	error = 0;
	while (!error && node && !(((t_token *)node->content)->type
			& (LPAREN | RPAREN | OR | SEMI | OR_IF | AND_IF | END)))
	{
		type = ((t_token *)node->content)->type;
		if (type & (LESS | DLESS | GREAT | DGREAT))
			error = setup_file_fds(&node, cmd, type);
		node = node->next;
	}
	return (error);
}

int	parse_expr(t_list *node, t_list **vars, t_cmd *cmd)
{
	int	i;
	int	error;

	i = 0;
	error = 0;
	cmd->path[0] = '\0';
	cmd->ior_start = NULL;
	while (!error && !(((t_token *)node->content)->type
			& (LPAREN | RPAREN | OR | SEMI | OR_IF | AND_IF | END)))
	{
		if (((t_token *)node->content)->type & (CMD | OPTS1 | OPTS2 | ARGS))
			cmd->opts[i++] = ((t_token *)node->content)->data;
		if ((((t_token *)node->content)->type & (CMD | BUILTIN)) == CMD)
			get_cmd_path(cmd->opts[0], cmd->path, *vars);
		if (((t_token *)node->content)->type == DLESS)
			error = get_heredoc(node->next->content, "> ", *vars);
		if (((t_token *)node->content)->type & (LESS | GREAT | DLESS | DGREAT)
			&& !cmd->ior_start)
			cmd->ior_start = node;
		node = node->next;
	}
	cmd->opts[i] = NULL;
	return (error);
}
