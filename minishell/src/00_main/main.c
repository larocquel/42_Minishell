/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:51:41 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/09 16:48:54 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig = 0;

int	main(int ac, char **av, char **envp)
{
	t_shell	sh;

	(void)av;
	if (ac > 1)
		fprintf(stderr, "minishell: args ignored\n");
	ms_init(&sh, envp);
	ms_loop(&sh);
	ms_destroy(&sh);
	return (sh.last_status);
}
