/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:52:59 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/10 15:39:59 by davmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(t_shell *sh, char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		ms_unsetenv(sh, argv[i]);
		i++;
	}
	return (0);
}
