/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 21:03:32 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/09 21:03:32 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# define MAX_SIZE	1024
# define MAX_OPTS	4098

# define E_MLOC "minishell: malloc: %s\n"
# define E_DUPE	"minishell: dup: %s\n"
# define E_DUP2	"minishell: dup2: %s\n"
# define E_PIPE	"minishell: pipe: %s\n"
# define E_FORK	"minishell: fork: %s\n"
# define E_WAIT	"minishell: wait: %s\n"
# define E_CMDE	"minishell: %s: command not found\n"
# define E_CMDS	"minishell: %s: %s\n"
# define E_AMBI	"minishell: %s: ambiguous redirect\n"
# define E_FILE	"minishell: %s: %s\n"
# define E_EOFW	"minishell: warning: here-document delimited by end-of-file \
(wanted `%s')\n"
# define E_EXPO	"minishell: export: `%s': not a valid identifier\n"
# define E_CHDR "minishell: cd: %s: %s\n"
# define E_CDAG "minishell: cd: too many arguments\n"
# define E_PWDA "minishell: pwd: too many arguments\n"
# define E_PWDV "minishell: pwd: %s\n"
# define E_ENVA "minishell: env: `%s': No such file or directory\n"
# define E_EXTA "minishell: exit: too many arguments\n"
# define E_EXTN "minishell: exit: %s: numeric argument required\n"

extern int	g_errno;

enum e_token_types {
	WORD	= 0x000000,
	CMD		= 0x000001,
	OPTS1	= 0x000002,
	OPTS2	= 0x000004,
	ARGS	= 0x000008,
	FILES	= 0x000010,
	WILD	= 0x000020,
	QUOT	= 0x000040,
	BUILTIN	= 0x000080,
	SPACES	= 0x000100,
	END		= 0x000200,
	LPAREN	= 0x000400,
	RPAREN	= 0x000800,
	SEMI	= 0x001000,
	LESS	= 0x002000,
	GREAT	= 0x004000,
	OR		= 0x008000,
	AND		= 0x010000,
	DLESS	= 0x020000,
	DGREAT	= 0x040000,
	OR_IF	= 0x080000,
	AND_IF	= 0x100000
};

enum e_state {
	DEFAULT,
	SQUOTE,
	DQUOTE
};

typedef struct s_token {
	int		type;
	char	*data;
}	t_token;

typedef struct s_ast {
	t_list			*expr;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_cmd {
	int		pid;
	int		fd[3];
	int		child;
	char	path[MAX_SIZE];
	char	*opts[MAX_OPTS];
	char	**envs;
	t_list	*ior_start;
}	t_cmd;

#endif
