/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_ops.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lla-rocq <lla-rocq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 21:57:37 by leoaguia          #+#    #+#             */
/*   Updated: 2026/01/05 22:21:38 by lla-rocq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Builtin: cd
*/
int	ft_cd(t_shell *sh, t_cmd *cmd)
{
	char	old_pwd[4096];
	char	*path;

	if (!getcwd(old_pwd, 4096))
		old_pwd[0] = '\0';
	if (!cmd->argv[1])
	{
		path = get_env_value(sh->env_list, "HOME");
		if (!path)
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			return (1);
		}
	}
	else
		path = cmd->argv[1];
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(path);
		return (1);
	}
	update_wd_env(sh, old_pwd);
	return (0);
}

/*
Auxiliar local para processar cada argumento do export
*/
static void	process_export_arg(t_shell *sh, char *arg)
{
	char	*eq_pos;

	eq_pos = ft_strchr(arg, '=');
	if (eq_pos)
	{
		*eq_pos = '\0';
		env_update(sh, arg, eq_pos + 1);
		*eq_pos = '=';
	}
	else
		env_update(sh, arg, NULL);
}

/*
Builtin: export
*/
int	ft_export(t_shell *sh, t_cmd *cmd)
{
	int	i;

	i = 1;
	if (!cmd->argv[1])
		return (ft_env(sh));
	while (cmd->argv[i])
	{
		if (!is_valid_key(cmd->argv[i]))
		{
			print_export_error(cmd->argv[i]);
			sh->last_status = 1;
		}
		else
			process_export_arg(sh, cmd->argv[i]);
		i++;
	}
	return (sh->last_status);
}

/*
Builtin: unset
Uso: unset VAR1 VAR2 ...
*/
int	ft_unset(t_shell *sh, t_cmd *cmd)
{
	int	i;

	i = 1;
	while (cmd->argv[i])
	{
		env_remove_node(sh, cmd->argv[i]);
		i++;
	}
	return (0);
}