/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:26:07 by leoaguia          #+#    #+#             */
/*   Updated: 2025/12/09 17:08:35 by leoaguia         ###   ########.fr       */
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
Nota: Futuramente vai lidar com argumentos (ex: exit 42) e limpar memória.
*/
int	ft_exit(t_shell *sh)
{
	(void)sh;	// Usaremos para free no futuro
	printf("exit\n");
	//	TODO: Chamar função de limpeza total (free_shell) aqui antes de sair
	exit(0);	// O que faz exit()?
	return (0);	// Nunca chega aqui
}