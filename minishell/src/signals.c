/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 19:37:35 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/18 23:05:42 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// DEFINIÇÃO única da variável global
volatile sig_atomic_t	g_signal = 0;

/*
Handler para SIGINT (Ctrl-C) no modo interativo.
Ideia:
- Imprime uma quebra de linha
- Limpa a linha atual do readline
- Mostra um novo prompt
*/
static void	sigint_handler(int sig)
{
	(void)sig;
	g_signal = CTRL_C;				// CTRL_C = 130 = exit status padrão de CTRL-C
	write(1, "^C\n", 3);
	rl_replace_line("", 0);			// Limpa o que foi digitado
	rl_on_new_line();				// Vamos para uma nova linha
	rl_redisplay();					// Redesenha o prompt (minishell$ )
}

/*
No modo interativo, queremos IGNORAR completamente o CTRL-\
Então vamos usar SIG_IGN em vez de um handler vazio.
*/
void	setup_signals_interactive(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;            // sem SA_RESTART, readline já lida bem com isso

	sa_quit.sa_handler = SIG_IGN;   // ignora CTRL-'\'
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;

	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);

	// Evita o readline instalar handlers próprios
	rl_catch_signals = 0;
}

