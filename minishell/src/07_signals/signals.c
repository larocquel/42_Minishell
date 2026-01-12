/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 19:37:35 by leoaguia          #+#    #+#             */
/*   Updated: 2026/01/12 19:03:59 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal;

/*
Handler for SIGINT (Ctrl-C) in interactive mode.
*/
static void	handle_sigint(int sig)
{
	(void)sig;
	g_signal = 130;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*
Configures signals for Prompt (Interactive Parent).
*/
void	setup_signals_interactive(void)
{
	struct sigaction	sa;

	signal(SIGQUIT, SIG_IGN);
	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &handle_sigint;
	sigaction(SIGINT, &sa, NULL);
}

/*
Restores default behavior for Child Processes.
*/
void	setup_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/*
Ignores signals in Parent while waiting for Children.
*/
void	setup_signals_ignore(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
