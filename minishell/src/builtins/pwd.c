/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davmendo <davmendo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:52:56 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/05 22:57:49 by davmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(char **argv)
{
	/* ponteiro atual para receber o caminho de getcwd */
	char	*cwd;

	(void)argv;
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		perror("pwd");
		return (1);
	}
	write(1, cwd, strlen(cwd));
	write(1, "\n", 1);
	free(cwd);
	return (0);
}
