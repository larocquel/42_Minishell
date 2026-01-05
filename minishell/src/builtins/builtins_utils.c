/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/28 14:25:17 by leoaguia          #+#    #+#             */
/*   Updated: 2025/12/28 15:11:13 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Auxiliar para ft_exit: Verifica se string é número
*/
int		is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/*
Verifica flag -n.
Retorna 1 se for -n válido (ex: -n, -nnnn), 0 caso contrário.
*/
int		check_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

/*
Helper para o CD:
Atualiza OLDPWD com o caminho antigo (old) e PWD com o atual.
*/
void	update_wd_env(t_shell *sh, char *old)
{
	char	cwd[4096];

	if (old) // Se conseguiu pegar o antigo, atualiza OLDPWD
		env_update(sh, "OLDPWD", old);
	if (getcwd(cwd, 4096)) // Pega o novo e atualiza PWD
		env_update(sh, "PWD", cwd);
}

/*
Helper para export: Imprime erro de identificador inválido
*/
void	print_export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
}

/*
Verifica se uma string é um identificador válido para variáveis de ambiente.
Regras do Bash:
1. Deve começar com letra ou sublinhado (_).
2. O restante pode conter letras, números ou sublinhados.
3. Para o 'export', paramos a verificação se encontrarmos um '='.
*/
int		is_valid_key(char *str)
{
	int	i;

	i = 0;
	// Verifica o primeiro caractere (não pode ser número)
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i++;
	// Verifica o resto até acabar a string ou achar um '='
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
