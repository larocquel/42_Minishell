/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:52:38 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/02 22:52:39 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(char **argv)
{
	int	i;
	int	nl;

	i = 1;
	nl = 1;
	if (argv[1] && !strcmp(argv[1], "-n"))
	{
		nl = 0;
		i = 2;
	}
	while (argv[i])
	{
		if (i > (nl ? 1 : 2)) write(1, " ", 1);
		write(1, argv[i], strlen(argv[i]));
		i++;
	}
	if (nl) write(1, "\n", 1);
	return (0);
}
