/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:52:34 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/12 16:23:44 by davmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#ifndef PATH_MAX
# define PATH_MAX 4096
#endif

/*
 * atualiza as variaveis do ambiente PWD e OLDPWD
*/

static int	update_pwd(t_shell *sh, char *old_pwd)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("minishell: cd: error retrieving current directory");
		return (1);
	}
	if (old_pwd)
		ms_setenv(sh, "OLDPWD", old_pwd);
	ms_setenv(sh, "PWD", cwd);
	return (0);
}

static int	cd_error(char *path)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
	return (1);
}

int	builtin_cd(t_shell *sh, char **argv)
{
	char	*path;
	char	old_pwd[PATH_MAX];
	char	*saved_pwd;

	if (argv[1] && argv[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
		return (1);
	}
	saved_pwd = NULL;
	if (getcwd(old_pwd, sizeof(old_pwd)))
		saved_pwd = old_pwd;
	if (!argv[1])
	{
		path = ms_getenv(sh->envp, "HOME");
		if (!path || !*path)
			return (ft_putendl_fd("minishell: cd: HOME not set", 2), 1);
	}
	else
		path = argv[1];
	if (chdir(path) != 0)
		return (cd_error(path));
	return (update_pwd(sh, saved_pwd));
}
