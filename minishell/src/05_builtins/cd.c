/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:52:34 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/16 19:20:27 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ms_set_pwd_vars(t_ms *sh, const char *oldpwd)
{
	char	buf[4096];

	if (!getcwd(buf, sizeof(buf))) return (perror("cd"), 1);
	ms_setenv(sh, "OLDPWD", oldpwd);
	ms_setenv(sh, "PWD", buf);
	return (0);
}

int	builtin_cd(t_ms *sh, char **argv)
{
	const char	*target;
	char		oldpwd[4096];

	target = argv[1];
	if (!getcwd(oldpwd, sizeof(oldpwd))) oldpwd[0] = '\0';
	if (!target) target = ms_getenv(sh->envp, "HOME");
	if (!target || !*target)
		return (fprintf(stderr, "minishell: cd: HOME not set\n"), 1);
	if (chdir(target) != 0)
		return (fprintf(stderr, "minishell: cd: %s: %s\n", target,
			strerror(errno)), 1);
	return (ms_set_pwd_vars(sh, oldpwd));
}
