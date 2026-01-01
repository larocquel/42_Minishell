/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:26:07 by leoaguia          #+#    #+#             */
/*   Updated: 2026/01/01 19:36:05 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Builtin: exit
Sai do shell, limpando toda a memória alocada.
Trata argumentos numéricos e erros de sintaxe.
*/
int ft_exit(t_shell *sh, t_cmd *cmd)
{
	int status;

	ft_putendl_fd("exit", 2);
	if (cmd->argv[1])
	{
		if (!is_numeric(cmd->argv[1]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(cmd->argv[1], 2);
			ft_putendl_fd(": numeric argument required", 2);
			// Limpeza completa antes de sair com erro
            free_env_list(sh->env_list);
			if (sh->cmds)
				free_cmds(sh->cmds);     // Limpa comandos atuais
			if (sh->tokens)
				free_tokens(sh->tokens); // Limpa tokens atuais
			rl_clear_history();
			exit(2);
		}
		if (cmd->argv[2])
		{
			ft_putendl_fd("minishell: exit: too many arguments", 2);
			return (1);
		}
		status = ft_atoi(cmd->argv[1]);
	}
	else
		status = sh->last_status;
	// Limpeza completa antes de sair normalmente
	free_env_list(sh->env_list);
	if (sh->cmds)
		free_cmds(sh->cmds);		// Limpa comandos atuais
	if (sh->tokens)
		free_tokens(sh->tokens);	// Limpa tokens atuais
	rl_clear_history();
	exit(status);
	return (0);
}

/*
Builtin: env
Imprime variáveis exportadas (que têm valor).
*/
int	ft_env(t_shell *sh)
{
	t_env	*tmp;

	tmp = sh->env_list;
	while (tmp)
	{
		if (tmp->value)
		{
			ft_putstr_fd(tmp->key, 1);
			ft_putstr_fd("=", 1);
			ft_putendl_fd(tmp->value, 1);
		}
		tmp = tmp->next;
	}
	return (0);
}

/*
Builtin: pwd
Imprime diretório atual.
*/
int	ft_pwd(void)
{
	char	cwd[4096];
	if (getcwd(cwd, 4096))
	{
		ft_putendl_fd(cwd, 1);
		return (0);
	}
	perror("minishell: pwd");
	return (1);
}

/*
Builtin: echo (Requer helper check_n_flag, que pode ficar aqui ou num utils)
*/
int	ft_echo(t_cmd *cmd)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;

	// check_n_flag deve estar definido (pode ser static neste arquivo)
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
