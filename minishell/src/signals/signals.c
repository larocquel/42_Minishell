/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 19:37:35 by leoaguia          #+#    #+#             */
/*   Updated: 2025/12/28 01:07:39 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Definição da global com o tipo seguro para sinais.
volatile: Obriga leitura da RAM (não cacheia em registrador).
sig_atomic_t: Garante que a escrita não seja interrompida pela metade.
*/
volatile sig_atomic_t	g_signal;

/*
Handler para SIGINT (Ctrl-C) no modo interativo.
1. Imprime nova linha.
2. Avisa ao readline que estamos em uma nova linha.
3. Limpa o buffer atual.
4. Redesenha o prompt.
*/
static void	handle_sigint(int sig)
{
	(void)sig;
	g_signal = 130;					// 130 = exit status padrão de CTRL-C
	write(1, "\n", 1);
	rl_on_new_line();				// Vamos para uma nova linha
	rl_replace_line("", 0);			// Limpa o que foi digitado
	rl_redisplay();					// Redesenha o prompt (minishell$ )
}

/*
Configura sinais para o Prompt (Pai Interativo).
*/
void	setup_signals_interactive(void)
{
	struct sigaction	sa;

	// Ignora SIGQUIT (Ctrl-\)
	signal(SIGQUIT, SIG_IGN);

	// Configura SIGINT (Ctrl-C)
	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &handle_sigint;
	sigaction(SIGINT, &sa, NULL);
}

/*
Restaura comportamento padrão para Processos Filhos.
*/
void	setup_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/*
Ignora sinais no Pai enquanto espera Filhos (Blocking).
*/
void	setup_signals_ignore(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

