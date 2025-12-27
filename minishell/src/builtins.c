/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:26:07 by leoaguia          #+#    #+#             */
/*   Updated: 2025/12/27 22:20:34 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Builtin: env
Comportamento: Imprime as variáveis de ambiente que têm valor.
Retorno: 0 (sucesso)
*/
int		ft_env(t_shell *sh)
{
	t_env	*tmp;

	tmp = sh->env_list;
	while (tmp)
	{
		// Bash padrão: se value for NULL (apenas exportada), não imprime no env
		if (tmp->value)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}

/*
Builtin: pwd
Comportamento: Imprime o diretório atual de trabalho.
Usa getcwd (permitida no subject).
*/
int	ft_pwd(void)
{
	char	cwd[1024];	// Buffer estático de tamanho rezaoável

	if (getcwd(cwd,sizeof(cwd)) != NULL)	// O que faz getcwd()?
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror("pwd");	// Imprime erro se falhar (ex: permissão)
		return (1);
	}
}

/*
Builtin: exit
Comportamento: Sai do shell.
Agora aceita argumentos para processar "exit 42".*/
int	ft_exit(t_shell *sh, t_cmd *cmd)
{
	int	status;

	printf("exit\n");
	status = 0;

	// Se tiver argumento (ex: exit 42), usa ele como status
	if (cmd && cmd->argv && cmd->argv[1])
		status = ft_atoi(cmd->argv[1]);
	else
		status = sh->last_status; // Se não tiver, sai com o status do último comando

	// TODO: Aqui entrará a função free_shell(sh) futuramente
	// free_env(sh->env_list); ...

	exit(status);
	return (0);
}

/*
Verifica flag -n.
Retorna 1 se for -n válido (ex: -n, -nnnn), 0 caso contrário.
*/
static int	check_n_flag(char *arg)
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
Builtin: echo
Opção -n: não imprime quebra de linha.
*/
int	ft_echo(t_cmd *cmd)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	// Pula flags -n iniciais (pode ter várias: echo -n -n -n oi)
	while (cmd->argv[i] && check_n_flag(cmd->argv[i]))
	{
		n_flag = 1;
		i++;
	}
	while (cmd->argv[i])
	{
		ft_putstr_fd(cmd->argv[i], 1);
		if (cmd->argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (n_flag == 0)
		write(1, "\n", 1);
	return (0);
}