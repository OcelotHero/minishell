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
	struct sigaction	ign_act;
	struct sigaction	rst_act;

	sigemptyset(&ign_act.sa_mask);
	ign_act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &ign_act, NULL);
	sigemptyset(&rst_act.sa_mask);
	rst_act.sa_handler = reset_action;
	sigaction(SIGINT, &rst_act, NULL);
}
