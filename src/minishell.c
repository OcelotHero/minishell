/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 02:56:31 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/07 02:56:31 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>

void	clear(void *content)
{
	t_token	*token;

	token = (t_token *)content;
	// printf("%05x, `%s`\n", token->type, token->data);
	if (token->data)
		free(token->data);
	free(content);
}

int	main(int narg, char **args, char **envs)
{
	int		res;
	char	**line;
	t_list	*var_list;
	t_list	*token_list;
	t_list	*tmp;
	t_ast	*ast;

	var_list = NULL;
	token_list = NULL;
	ast = NULL;
	line = (char *[]){NULL, NULL};
	while (*envs)
		ft_lstadd_front(&var_list, ft_lstnew(ft_strdup(*(envs++))));
	while (true)
	{
		line[0] = readline("$minishell>");
		if (ft_strlen(line[0]) && !tokenize(&token_list, line[0]))
		{
			if (!line[1] || ft_strcmp(line[0], line[1]))
				add_history(line[0]);
			preprocess_token(token_list, var_list);
			tmp = token_list;
			res = expr(&tmp, &ast);
			if (res == 2 && ((t_token *)tmp->content)->type == END)
				printf("syntax error near unexpected token `newline'\n");
			else if (res != 1 && ((t_token *)tmp->content)->type != END)
				printf("syntax error near unexpected token `%s'\n",
						((t_token *)tmp->content)->data);
			if (ast)
				interpret_ast(ast, &var_list);
			ast_clear(&ast, NULL);
			ft_lstclear(&token_list, clear);
		}
		if (line[1])
			free(line[1]);
		line[1] = line[0];
	}
	ft_lstclear(&var_list, free);
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <glob.h>

// int main()
// {
// 	char **found;
// 	glob_t gstruct;
// 	int r;

// 	r = glob("\\*.c", GLOB_ERR , NULL, &gstruct);
// 	/* check for errors */
// 	if( r!=0 )
// 	{
// 		if( r==GLOB_NOMATCH )
// 			fprintf(stderr,"No matches\n");
// 		else
// 			fprintf(stderr,"Some kinda glob error\n");
// 		exit(1);
// 	}

// 	/* success, output found filenames */
// 	printf("Found %zu filename matches\n",gstruct.gl_pathc);
// 	found = gstruct.gl_pathv;
// 	while(*found)
// 	{
// 		printf("%s\n",*found);
// 		found++;
// 	}

// 	return(0);
// }
