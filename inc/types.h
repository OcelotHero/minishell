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
# define MAX_OPTS	1024

enum e_token_types {
	WORD	= 0x00000,
	CMD		= 0x00001,
	OPTS1	= 0x00002,
	OPTS2	= 0x00004,
	ARGS	= 0x00008,
	FILES	= 0x00010,
	WILD	= 0x00020,
	SPACES	= 0x00040,
	END		= 0x00080,
	LPAREN	= 0x00100,
	RPAREN	= 0x00200,
	SEMI	= 0x00400,
	LESS	= 0x00800,
	GREAT	= 0x01000,
	OR		= 0x02000,
	AND		= 0x04000,
	DLESS	= 0x08000,
	DGREAT	= 0x10000,
	OR_IF	= 0x20000,
	AND_IF	= 0x40000
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
	char	path[MAX_SIZE];
	char	*opts[MAX_OPTS];
	t_list	*ior_start;
}	t_cmd;

#endif
