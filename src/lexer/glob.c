/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 02:45:01 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/13 02:45:01 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <glob.h>

int main()
{
    // char **found;
    // glob_t gstruct;
    // int r;

    // r = glob("../parser/*", GLOB_ERR , NULL, &gstruct);
    // /* check for errors */
    // if( r!=0 )
    // {
    //     if( r==GLOB_NOMATCH )
    //         fprintf(stderr,"No matches\n");
    //     else
    //         fprintf(stderr,"Some kinda glob error\n");
    //     exit(1);
    // }
    
    // /* success, output found filenames */
    // printf("Found %zu filename matches\n",gstruct.gl_pathc);
    // found = gstruct.gl_pathv;
    // while(*found)
    // {
    //     printf("%s\n",*found);
    //     found++;
    // }
	// globfree(&gstruct);

    // return(0);
    int     i = 0;
    char    *s;
    char    *c;

    *s = 'a';
    c = (char *)(long)s;
    printf("%p, %ld, %c\n", s, (long)s, *c);
}