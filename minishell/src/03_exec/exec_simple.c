/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 17:57:07 by leoaguia          #+#    #+#             */
/*   Updated: 2026/01/08 18:37:52 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Função auxiliar para encerrar processo filho limpando TUDO.
*/
void	exit_child(t_shell *sh, int status)
{
	free_env_list(sh->env_list);
	if (sh->tokens)
		free_tokens(sh->tokens);
	if (sh->cmds)
		free_cmds(sh->cmds);
	rl_clear_history();
	exit(status);
}

/*
Lida com erros de execução baseados no errno.
*/
static void	handle_exec_error(char *cmd, t_shell *sh)
{
	int	exit_code;

	exit_code = 1;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	if (errno == ENOENT)
	{
		ft_putendl_fd(": command not found", 2);
		exit_code = 127;
	}
	else if (errno == EACCES)
	{
		ft_putendl_fd(": permission denied", 2);
		exit_code = 126;
	}
	else
	{
		ft_putendl_fd(": exec error", 2);
		perror("");
	}
	exit_child(sh, exit_code);
}

/*
Verifica e executa builtins dentro do processo filho.
CORRECAO: O export agora eh executado, pois export sem args imprime coisas.
*/
static void	check_builtin_child(t_shell *sh, t_cmd *cmd)
{
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		exit_child(sh, ft_echo(cmd));
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		exit_child(sh, ft_pwd());
	if (ft_strcmp(cmd->argv[0], "env") == 0)
		exit_child(sh, ft_env(sh));
	if (ft_strcmp(cmd->argv[0], "export") == 0)
		exit_child(sh, ft_export(sh, cmd));
	if (ft_strcmp(cmd->argv[0], "unset") == 0
		|| ft_strcmp(cmd->argv[0], "cd") == 0)
		exit_child(sh, 0);
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
		exit_child(sh, ft_exit(sh, cmd));
}

/*
Executa um comando simples.
*/
void	exec_simple_cmd(t_shell *sh, t_cmd *cmd)
{
	char	**env_arr;
	char	*path;

	if (!cmd->argv || !cmd->argv[0])
		exit_child(sh, 0);
	check_builtin_child(sh, cmd);
	if (ft_strchr(cmd->argv[0], '/'))
		path = ft_strdup(cmd->argv[0]);
	else
		path = find_executable(cmd->argv[0], sh->env_list);
	if (!path)
	{
		errno = ENOENT;
		handle_exec_error(cmd->argv[0], sh);
	}
	env_arr = env_to_array(sh->env_list);
	execve(path, cmd->argv, env_arr);
	free(path);
	free_array(env_arr);
	handle_exec_error(cmd->argv[0], sh);
}
