/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_ops.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 21:57:37 by leoaguia          #+#    #+#             */
/*   Updated: 2025/12/13 12:54:51 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Verifica validade do nome da variável (apenas alfanumérico e _) */
static int	is_valid_key(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')	// Paramos no '=' se houver
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/*
Builtin: export
Sintaxe: export VAR=valor OU export VAR
*/
int	ft_export(t_shell *sh, t_cmd *cmd)
{
	int		i;
	char	*equal_sign;
	char	*key;
	char	*val;

	i = 1;
	// Se rodar apenas "export", bash lista vars. Para o mandatório, podemos ignorar ou usar ft_env.
	if (!cmd->argv[1])
		return (ft_env(sh));
	while (cmd->argv[i])
	{
		if (!is_valid_key(cmd->argv[i]))
		{
			printf("export: `%s': not a valid identifier\n", cmd->argv[i]);
			sh->last_status = 1;	// Erro não fatal
		}
		else
		{
			// Procura o '='
			equal_sign = ft_strchr(cmd->argv[i], '=');
			if (equal_sign)
			{
				// Caso VAR=valor
				*equal_sign = '\0';	// Divide temporariamente na posição do =
				key = cmd->argv[i];
				val = equal_sign + 1;
				env_update(sh, key, val);
				*equal_sign = '=';	// Restaura

			}
			else
			{
				// Caso VAR (sem valor)
				env_update(sh, cmd->argv[i], NULL);
			}
		}
		i++;
	}
	return (0);
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
		// Não precisa checar validade estrita no unset, mas é bom hábito.
		// Bash ignora silenciosamente se não existir.
		env_remove_node(sh, cmd->argv[i]);
		i++;
	}
	return (0);
}

/* Auxiliar do CD para atualizar OLDPWD e PWD */
static void	update_pwds(t_shell *sh)
{
	char	cwd[1024];
	char	*old;

	// Pega o valor atual de PWD para virar OLDPWD
	old = get_env_value(sh->env_list, "PWD");
	if (old)
		env_update(sh, "OLDPWD", old);

	// Pega o diretório atual do sistema e atualiza PWD
	if (getcwd(cwd, sizeof(cwd)))
		env_update(sh, "PWD", cwd);
}

/* Builtin: cd */
int	ft_cd(t_shell *sh, t_cmd *cmd)
{
	char *path;

	// Se não tiver argumentos, vai para HOME
	if (!cmd->argv[1])
	{
		path = get_env_value(sh->env_list, "HOME");
		if (!path)
		{
			printf("cd: HOME not set\n");
			return (1);
		}
	}
	else
		path = cmd->argv[1];

	// Tenta mudar o diretório
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}

	// Se deu certo, atualiza variáveis
	update_pwds(sh);
	return (0);
}