/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 16:53:55 by leoaguia          #+#    #+#             */
/*   Updated: 2025/12/28 01:40:03 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Lida com erros de execução baseados no errno.
ENOENT (2): Arquivo ou diretório inexistente -> Status 127
EACCES (13): Permissão negada ou é diretório -> Status 126
*/
static void	handle_exec_error(char *cmd)
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
	exit(exit_code);
}

/*
Executa um comando simples (sem pipe) ou o último de uma pipeline.
*/
void	exec_simple_cmd(t_shell *sh, t_cmd *cmd)
{
	char	**env_arr;
	char	*path;

	// 1. Builtins
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		exit(ft_echo(cmd));
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		exit(ft_pwd());
	if (ft_strcmp(cmd->argv[0], "env") == 0)
		exit(ft_env(sh));
	if (ft_strcmp(cmd->argv[0], "export") == 0 || \
		ft_strcmp(cmd->argv[0], "unset") == 0 || \
		ft_strcmp(cmd->argv[0], "cd") == 0)
		exit(0);
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
		exit(ft_exit(sh, cmd));

	// 2. Resolução do Caminho
	// Se tiver barra '/', é caminho direto. Se não, busca no PATH.
	if (ft_strchr(cmd->argv[0], '/'))
		path = ft_strdup(cmd->argv[0]);
	else
		path = find_executable(cmd->argv[0], sh->env_list);

	// 3. Verificação de existência antes do execve
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}

	// 4. Execução
	env_arr = env_to_array(sh->env_list);
	execve(path, cmd->argv, env_arr);

	// 5. Tratamento de erro do execve (Permissão, Diretório, etc)
	free(path);
	free_array(env_arr);
	handle_exec_error(cmd->argv[0]);
}

static void	child_process(t_shell *sh, t_cmd *cmd, int fd_in, int fd_out)
{
	if (fd_in != -1)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (fd_out != -1)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	if (setup_redirects(cmd) != 0)
		exit(1);
	exec_simple_cmd(sh, cmd);
}

/*
Aguarda o último processo especificamente para pegar o status correto ($?).
Depois aguarda todos os outros para evitar zumbis.
*/
static void	wait_children(t_shell *sh, int last_pid)
{
	int	status;

	// Espera o último comando da pipe (o que importa pro $?)
	waitpid(last_pid, &status, 0);
	if (WIFEXITED(status))
		sh->last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		sh->last_status = 128 + WTERMSIG(status);

	// Limpa os outros processos filhos que sobraram
	while (waitpid(-1, NULL, 0) > 0)
		continue ;
}

/*
Gerencia a criação de pipes e forks.
Salva o PID do último comando para capturar o status correto depois.
*/
void	execute_pipeline(t_shell *sh, t_cmd *cmds)
{
	int		pipefd[2];
	int		fd_in;
	pid_t	pid;
	t_cmd	*tmp;

	fd_in = -1;
	setup_signals_ignore();
	tmp = cmds;
	while (tmp)
	{
		if (tmp->next && pipe(pipefd) == -1)
			return (perror("pipe"));
		pid = fork();
		if (pid == 0)
		{
			setup_signals_child();
			if (tmp->next)
				close(pipefd[0]);
			child_process(sh, tmp, fd_in, (tmp->next ? pipefd[1] : -1));
		}
		if (tmp->next)
			close(pipefd[1]);
		if (fd_in != -1)
			close(fd_in);
		if (tmp->next)
			fd_in = pipefd[0];
		// Se for o último nó, o pid atual é o que nos interessa
		if (!tmp->next)
			wait_children(sh, pid);
		tmp = tmp->next;
	}
	setup_signals_interactive();
}