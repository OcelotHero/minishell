/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executables.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snagulap <snagulap@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:09:21 by snagulap          #+#    #+#             */
/*   Updated: 2023/08/07 14:48:24 by snagulap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interpret.h"
#include <sys/stat.h>

int	exec_builtin(t_list **vars, t_cmd *cmd)
{
	int	*fd;
	int	status;

	fd = (int []){dup(STDIN_FILENO), dup(STDOUT_FILENO)};
	if (fd[0] < 0 || fd[1] < 0)
	{
		if (fd[0] > 0)
			close(fd[0]);
		if (fd[1] > 0)
			close(fd[1]);
		return (error_msg(errno, E_DUPE, strerror(errno)));
	}
	status = exec(vars, cmd);
	if (dup2(fd[0], STDIN_FILENO) < 0 || dup2(fd[1], STDOUT_FILENO) < 0)
		status = error_msg(errno, E_DUP2, strerror(errno));
	close(fd[0]);
	close(fd[1]);
	return (status);
}

int	exec_child(t_list *node, t_list **vars, t_cmd *cmd, int flags)
{
	int	status;

	if (parse_expr(node, vars, cmd) || !(flags & 1))
		return (flags & 1);
	if (!cmd->child && is_builtin(cmd->opts[0]))
		return (exec_builtin(vars, cmd));
	ft_swap(&cmd->fd[0], &cmd->fd[2]);
	cmd->pid = fork();
	if (cmd->pid < 0 || (!cmd->pid && exec(vars, cmd)))
		return (!cmd->pid || error_msg(errno, E_FORK, strerror(errno)));
	if (cmd->fd[0] && (close(cmd->fd[0]) || 1))
		cmd->fd[0] = 0;
	if (cmd->fd[1] && (close(cmd->fd[1]) || 1))
		cmd->fd[1] = 0;
	if (!(flags & 2) || !cmd->pid)
		return (!cmd->pid);
	if (cmd->fd[2] && (close(cmd->fd[2]) || 1))
		cmd->fd[2] = 0;
	if (waitpid(cmd->pid, &status, 0) < 0)
		return (error_msg(errno, E_WAIT, strerror(errno)));
	while (cmd->child && cmd->child--)
		if (waitpid(-1, NULL, 0) < 0)
			return (error_msg(errno, E_WAIT, strerror(errno)));
	return (error_msg(WEXITSTATUS(status) | WTERMSIG(status)
			| WIFSIGNALED(status) * 128, ""));
}

int	execute_cmd(t_list **vars, t_cmd *cmd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cmd->opts[0] && !ft_strccmp(cmd->opts[0], "cd", !LINUX))
		return (builtin_cd(cmd->opts, vars));
	if (cmd->opts[0] && !ft_strccmp(cmd->opts[0], "echo", !LINUX))
		return (builtin_echo(cmd->opts, vars));
	if (cmd->opts[0] && !ft_strccmp(cmd->opts[0], "env", !LINUX))
		return (builtin_env(cmd->opts, vars));
	if (cmd->opts[0] && !ft_strccmp(cmd->opts[0], "exit", 0))
		return (builtin_exit(cmd->opts, vars));
	if (cmd->opts[0] && !ft_strccmp(cmd->opts[0], "export", 0))
		return (builtin_export(cmd->opts, vars));
	if (cmd->opts[0] && !ft_strccmp(cmd->opts[0], "pwd", !LINUX))
		return (builtin_pwd(cmd->opts, vars));
	if (cmd->opts[0] && !ft_strccmp(cmd->opts[0], "unset", 0))
		return (builtin_unset(cmd->opts, vars));
	return (execve(cmd->path, cmd->opts, cmd->envs));
}

int	exec(t_list **vars, t_cmd *cmd)
{
	char		*s;
	struct stat	sstruct;

	if (cmd->fd[2] && (close(cmd->fd[2]) || 1))
		cmd->fd[2] = 0;
	if (setup_file_redirections(cmd->ior_start, cmd))
		return (1);
	if (((cmd->fd[0] && dup2(cmd->fd[0], STDIN_FILENO) < 0)
			|| (cmd->fd[1] && dup2(cmd->fd[1], STDOUT_FILENO) < 0))
		&& (cmd->fd[0] && close(cmd->fd[0]) || 1)
		&& (cmd->fd[1] && close(cmd->fd[1]) || 1))
		return (error_msg(errno, E_DUP2, strerror(errno)));
	if (cmd->fd[0] && (close(cmd->fd[0]) || 1))
		cmd->fd[0] = 0;
	if (cmd->fd[1] && (close(cmd->fd[1]) || 1))
		cmd->fd[1] = 0;
	if (!(cmd->path[0]) && cmd->opts[0] && !is_builtin(cmd->opts[0]))
		return (error_msg(127, E_CMDE, cmd->opts[0]));
	if (cmd->path[0] && (access(cmd->path, X_OK) || access(cmd->path, R_OK)))
		return (error_msg(126, E_CMDS, cmd->opts[0], strerror(errno)));
	if (cmd->path[0] && stat(cmd->path, &sstruct))
		return (error_msg(errno, E_STAT, strerror(errno)));
	if (cmd->path[0] && S_ISDIR(sstruct.st_mode))
		return (error_msg(126, E_DIRS, cmd->opts[0]));
	return (execute_cmd(vars, cmd));
}
