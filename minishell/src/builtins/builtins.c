/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davmendo <davmendo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:52:31 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/06 16:36:16 by davmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_builtin(t_shell *sh, char **argv)
{
	if (!strcmp(argv[0], "echo")) return (builtin_echo(argv));
	if (!strcmp(argv[0], "pwd")) return (builtin_pwd(argv));
	if (!strcmp(argv[0], "env")) return (builtin_env(sh, argv));
	if (!strcmp(argv[0], "exit")) return (builtin_exit(sh, argv));
	if (!strcmp(argv[0], "cd")) return (builtin_cd(sh, argv));
	if (!strcmp(argv[0], "export")) return (builtin_export(sh, argv));
	if (!strcmp(argv[0], "unset")) return (builtin_unset(sh, argv));
	return (1);
}
