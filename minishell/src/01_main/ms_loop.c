/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_loop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:51:46 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/16 19:20:22 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Atualiza a flag de modo interativo (para imprimir "exit" no Ctrl-D).
*/
static void	ms_update_interactive(t_ms *sh)
{
	sh->interactive = isatty(STDIN_FILENO);
}

/*
** Lê a linha do prompt; adiciona ao histórico se não for vazia.
** Retorna NULL em EOF (Ctrl-D).
*/
static char	*ms_read_prompt_line(t_ms *sh)
{
	char	*line;

	(void)sh;
	line = readline("minishell$ ");
	if (!line)
		return (NULL);
	if (*line)
		add_history(line);
	return (line);
}

/*
** Loop principal: lê uma linha e delega o processamento ao handle_line().
** Sai com "exit" em modo interativo quando readline retorna NULL.
*/
void	ms_loop(t_ms *sh)
{
	char	*line;

	ms_update_interactive(sh);
	signals_setup_interactive();
	while (1)
	{
		line = ms_read_prompt_line(sh);
		if (!line)
		{
			if (sh->interactive)
				printf("exit\n");
			break ;
		}
		handle_line(sh, line);
	}
}
