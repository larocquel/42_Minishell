/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:52:47 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/19 16:59:13 by davmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * verifica se e uma string com numero valido para o builtin exit
 * usada para validar o argumento antes de converte-lo
 * se nao for numerico, imprime o erro e sai com codigo 255
 * aceita um sinal '+' ou '-' somente nas primeiras posicoes
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

/*
 * verifica se a parte numerica da string tem overflow ou underflow
 * retorna 1 se houver overflow ou underflow
 * retorna 0 caso o contrario
*/

static int	check_overflow(const char *str, int sign)
{
	int		i;
	char	*limit;

	i = 0;
	if (sign == 1)
		limit = "9223372036854775807";
	else
		limit = "9223372036854775808";
	while (str[i] == '0')
		i++;
	if (ft_strlen(&str[i]) > 19)
		return (1);
	if (ft_strlen(&str[i]) < 19)
		return (0);
	if (ft_strncmp(&str[i], limit, 19) > 0)
		return (1);
	return (0);
}
/*
 * analisa a string do argumento de exit e converte para um long
 * retornando 1 em sucesso e 0 se detectar overflow
*/

static int	parse_exit_arg(const char *str, long long *result)
{
	int			i;
	int			sign;
	long long	res;

	i = 0;
	sign = 1;
	res = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (check_overflow(&str[i], sign))
		return (0);
	while (str[i])
		res = res * 10 + sign * (str[i++] - '0');
	*result = res;
	return (1);
}

static void	exit_with_error(t_shell *sh, char *arg, int code)
{
	if (sh->interactive)
		ft_putendl_fd("exit", STDERR_FILENO);
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	shell_destroy(sh);
	exit(code);
}

int	builtin_exit(t_shell *sh, char **argv)
{
	long long	code;

	if (argv[1] && !is_numeric(argv[1]))
		exit_with_error(sh, argv[1], 255);
	if (argv[1] && !parse_exit_arg(argv[1], &code))
		exit_with_error(sh, argv[1], 255);
	if (argv[1] && argv[2])
	{
		if (sh->interactive)
			ft_putendl_fd("exit", STDERR_FILENO);
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	if (sh->interactive)
		ft_putendl_fd("exit", STDERR_FILENO);
	if (!argv[1])
		code = sh->last_status;
	shell_destroy(sh);
	exit((int)(code & 0xFF));
}
