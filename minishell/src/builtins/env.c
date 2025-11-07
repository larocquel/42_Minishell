/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:52:41 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/07 17:00:44 by davmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* imprime variáveis de ambiente no formato KEY=VALUE.
* retorna 1 se receber argumentos a mais, caso contrário retorna 0
*/

int	builtin_env(t_shell *sh, char **argv)
{
	size_t	i;
	if (argv && argv[1])
	{
		ft_putendl_fd("minishell: env: too many arguments", STDERR_FILENO);
		return (1);
	}
	i = 0;
	while (sh->envp && sh->envp[i])
	{
		if (ft_strchr(sh->envp[i], '='))
			ft_putendl_fd(sh->envp[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}
