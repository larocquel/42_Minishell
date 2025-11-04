/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:52:47 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/02 22:52:48 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit(t_shell *sh, char **argv)
{
	long	code;

	code = sh->last_status;
	if (argv[1])
	{
		if (!ms_isnumber(argv[1]))
		{
			fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
				argv[1]);
			code = 2;
		}
		else if (argv[2])
			return (fprintf(stderr, "minishell: exit: too many arguments\n"), 1);
		else
			code = strtol(argv[1], NULL, 10) & 0xFF;
	}
	if (isatty(STDIN_FILENO)) printf("exit\n");
	shell_destroy(sh);
	_exit((int)code);
}
