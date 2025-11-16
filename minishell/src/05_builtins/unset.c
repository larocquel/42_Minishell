/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:52:59 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/16 19:20:30 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(t_ms *sh, char **argv)
{
	int	i;
	int	rc;

	i = 1;
	rc = 0;
	while (argv[i])
	{
		if (ms_unsetenv(sh, argv[i]) != 0) rc = 1;
		i++;
	}
	return (rc);
}
