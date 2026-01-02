/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 16:53:55 by leoaguia          #+#    #+#             */
/*   Updated: 2026/01/02 17:37:00 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* src/executor.c */
#include "minishell.h"

/*
Função auxiliar para encerrar processo filho limpando TUDO.
Isso remove o "Still Reachable" do Valgrind nos builtins.
*/
static void	exit_child(t_shell *sh, int status)
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
Atualizado para receber 'sh' e limpar memória antes de sair.
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
	exit_child(sh, exit_code); // <--- Limpeza aqui
}

/*
Executa um comando simples (sem pipe) ou o último de uma pipeline.
*/
void	exec_simple_cmd(t_shell *sh, t_cmd *cmd)
{
	char	**env_arr;
	char	*path;

	// 1. Builtins: Usamos exit_child em vez de exit direto
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		exit_child(sh, ft_echo(cmd));
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		exit_child(sh, ft_pwd());
	if (ft_strcmp(cmd->argv[0], "env") == 0)
		exit_child(sh, ft_env(sh));
	if (ft_strcmp(cmd->argv[0], "export") == 0 || \
		ft_strcmp(cmd->argv[0], "unset") == 0 || \
		ft_strcmp(cmd->argv[0], "cd") == 0)
		exit_child(sh, 0);
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
		ft_exit(sh, cmd); // ft_exit já tem limpeza interna, não precisa wrapper

	// 2. Resolução do Caminho
	if (ft_strchr(cmd->argv[0], '/'))
		path = ft_strdup(cmd->argv[0]);
	else
		path = find_executable(cmd->argv[0], sh->env_list);

	// 3. Verificação de existência
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit_child(sh, 127); // <--- Limpeza aqui
	}

	// 4. Execução
	env_arr = env_to_array(sh->env_list);
	execve(path, cmd->argv, env_arr);

	// 5. Tratamento de erro do execve
	free(path);
	free_array(env_arr);
	handle_exec_error(cmd->argv[0], sh); // <--- Passamos sh para limpar
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
		exit_child(sh, 1); // <--- Limpeza se redireção falhar
	exec_simple_cmd(sh, cmd);
}

static void	wait_children(t_shell *sh, int last_pid)
{
	int	status;

	waitpid(last_pid, &status, 0);
	if (WIFEXITED(status))
		sh->last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		sh->last_status = 128 + WTERMSIG(status);
	while (waitpid(-1, NULL, 0) > 0)
		continue ;
}

/* src/executor.c */

void	execute_pipeline(t_shell *sh, t_cmd *cmds)
{
	int		pipefd[2];
	int		fd_in;
	int		fd_out; // 1. Nova variável auxiliar
	pid_t	pid;
	t_cmd	*tmp;

	fd_in = -1;
	setup_signals_ignore();
	tmp = cmds;
	while (tmp)
	{
		if (tmp->next && pipe(pipefd) == -1)
		{
			perror("pipe");
			return ;
		}
		pid = fork();
		if (pid == 0)
		{
			setup_signals_child();
			if (tmp->next)
				close(pipefd[0]);

			// 2. Lógica substituindo o ternário
			if (tmp->next)
				fd_out = pipefd[1];
			else
				fd_out = -1;

			child_process(sh, tmp, fd_in, fd_out);
		}
		if (tmp->next)
			close(pipefd[1]);
		if (fd_in != -1)
			close(fd_in);
		if (tmp->next)
			fd_in = pipefd[0];
		if (!tmp->next)
			wait_children(sh, pid);
		tmp = tmp->next;
	}
	setup_signals_interactive();
}
