/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:52:47 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/12 14:59:04 by davmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * verifica se a string e um numero inteiro valido
 * pode comecar com '-' ou '+'
 * depois de tem pelo um digito
 * so pode ter digitos ate o fim
*/

static int	is_numeric(const char *s)
{
	int	i;

	if (!s || !s[0])
		return (0);
	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!ft_isdigit((unsigned char)s[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(t_shell *sh, char **argv)
{
	long	code;

	if (sh->interactive)
		ft_putendl_fd("exit", STDERR_FILENO);
	if (argv[1] && argv[2] && is_numeric(argv[1]))
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	if (argv[1] && !is_numeric(argv[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		code = 255;
	}
	else if (argv[1])
		code = ft_atoi(argv[1]);
	else
		code = sh->last_status;
	shell_destroy(sh);
	exit((int)(code & 0xFF));
}
