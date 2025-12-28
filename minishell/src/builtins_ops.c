/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_ops.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 21:57:37 by leoaguia          #+#    #+#             */
/*   Updated: 2025/12/28 15:57:55 by leoaguia         ###   ########.fr       */
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

	// 1. Salva onde estamos agora (será o OLDPWD)
	if (!getcwd(old_pwd, 4096))
		old_pwd[0] = '\0';	// Marca como vazio se falhar

	// 2. Define destino
	if (!cmd->argv[1])
	{
		path = get_env_value(sh->env_list, "HOME");
		if (!path)
		{
			ft_putendl_fd("minihsell: cd: HOME not set ", 2);
			return (1);
		}
	}
	else
		path = cmd->argv[1];

	// 3. Executa a mudança
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		perror("");	// Imprime erro do sistema
		return (1);
	}
	update_wd_env(sh, old_pwd);
	return (0);
}

/*
Builtin: export
*/
int	ft_export(t_shell *sh, t_cmd *cmd)
{
	int		i;
	char	*eq_pos;

	i = 1;
	if (!cmd->argv[1])
		return (ft_env(sh));	// export sem args lista vars (simplificação)
	while (cmd->argv[i])
	{
		if (!is_valid_key(cmd->argv[i]))
		{
			print_export_error(cmd->argv[i]);
			sh->last_status = 1;
		}
		else
		{
			eq_pos = ft_strchr(cmd->argv[i], '=');
			if (eq_pos)
			{
				*eq_pos = '\0';	// Corta string temporariamente
				env_update(sh, cmd->argv[i], eq_pos + 1);
				*eq_pos = '='; // Restaura
			}
			else	// Caso: export VAR (sem valor)
				env_update(sh, cmd->argv[i], NULL);
		}
		i++;
	}
	return (sh->last_status);	// Retorna erro se algum falhou
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