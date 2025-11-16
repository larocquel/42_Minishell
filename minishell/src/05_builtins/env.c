/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:52:41 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/16 19:20:28 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(t_ms *sh)
{
	size_t	i;

	i = 0;
	while (sh->envp && sh->envp[i])
	{
		if (strchr(sh->envp[i], '=')) printf("%s\n", sh->envp[i]);
		i++;
	}
	return (0);
}
