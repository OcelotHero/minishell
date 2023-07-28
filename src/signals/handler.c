/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rraharja <rraharja@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 00:44:09 by rraharja          #+#    #+#             */
/*   Updated: 2023/07/12 00:44:09 by rraharja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

void	reset_action(int signo)
{
	(void) signo;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signals(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &reset_action);
}
