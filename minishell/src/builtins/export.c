/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davmendo <davmendo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:30:50 by davmendo          #+#    #+#             */
/*   Updated: 2025/11/18 23:38:59 by davmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	*verifica se a string passada em arg e um nome identificador valido
	*nao pode ser null nem vazia
	*nao pode comcar com = nem numero
	*o primeiro caractere tem que ser letra ou '_'
	*os proximos caracteres ate encontrar = ou NULL 
	*tem que ser letra, numero ou '_'
*/
static int	ms_is_valid_identifier(const char *arg)
{
	size_t	i;

	if (arg == NULL || arg[0] == '\0')
		return (0);
	if (arg[0] == '=' || ft_isdigit((unsigned char)arg[0]))
		return (0);
	if (!ft_isalpha((unsigned char)arg[0]) && arg[0] != '_')
		return (0);
	i = 1;
	while (arg[i] != '\0' && arg[i] != '=')
	{
		if (!ft_isalnum((unsigned char)arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	ms_export_error(const char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd((char *)arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	return (1);
}
/*
	*processa o argumento passado para o export
	*e atualiza as variavies de ambiente
	*char *eq = equal, um ponteiro para posicao do caratecere '='
	*eq serve para separar KEY=VALUE na mesma string
	*ret = return, guarda o valor de retorno para sabe se deu erro ou nao
*/

static int	ms_export_process_arg(t_shell *sh, char *arg)
{
	char	*eq;
	int		ret;

	if (!ms_is_valid_identifier(arg))
		return (ms_export_error(arg));
	eq = ft_strchr(arg, '=');
	if (eq != NULL)
	{
		*eq = '\0';
		ret = ms_setenv(sh, arg, eq + 1);
		*eq = '=';
		if (ret < 0)
			return (1);
	}
	else
	{
		if (ms_export_mark_no_value(sh, arg) < 0)
			return (1);
	}
	return (0);
}
/*
	*percorre todos os argumentos, 
	*chama a funcao process_arg para cada arg e ajusta o last_status
	*i = 1 porque o argv[0] e o nome do comando
	*se qualquer argumentou falhou, exit status 1
	*se todos foram processados com sucesso = 0
	*retorna sh->last_status que e o codigo de saida builtin export
*/

static int	ms_export_with_args(t_shell *sh, char **argv)
{
	int	i;
	int	error;

	i = 1;
	error = 0;
	while (argv[i] != NULL)
	{
		if (ms_export_process_arg(sh, argv[i]) != 0)
			error = 1;
		i++;
	}
	if (error != 0)
		sh->last_status = 1;
	else
		sh->last_status = 0;
	return (sh->last_status);
}
/*
	*implementacao principal do comando export
	*decide qual comportamento usar dependendo dos argumentos
	*se algo estiver invalido retorna 1(erro)
	*se nao existe av[1] chama a funcao para listar as variaveis de ambiente
	*se existe av[1] percorre os argumentos com a funcao process_arg
*/

int	builtin_export(t_shell *sh, char **argv)
{
	if (sh == NULL || argv == NULL || argv[0] == NULL)
		return (1);
	if (argv[1] == NULL)
	{
		sh->last_status = ms_export_no_args(sh);
		return (sh->last_status);
	}
	return (ms_export_with_args(sh, argv));
}
