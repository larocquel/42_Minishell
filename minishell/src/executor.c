/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 16:53:55 by leoaguia          #+#    #+#             */
/*   Updated: 2025/12/27 21:43:48 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Executa um comando simples (sem pipe |) ou o ultimo de uma pipeline.
Pode ser builtin ou externo.
*/
void	exec_simple_cmd(t_shell *sh, t_cmd *cmd)
{
	char	**env_arr;
	char	*path;

	// 1. Intercepta Builtins dentro do filho
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		exit(ft_echo(cmd));
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		exit(ft_pwd());
	if (ft_strcmp(cmd->argv[0], "env") == 0)
		exit(ft_env(sh));
	if (ft_strcmp(cmd->argv[0], "export") == 0)
		exit(0); // Em pipe, export não faz nada, mas não deve dar erro
	if (ft_strcmp(cmd->argv[0], "unset") == 0)
		exit(0);
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		exit(0);
	// Exit no pipe também não faz sentido, sai só o filho
    if (ft_strcmp(cmd->argv[0], "exit") == 0)
		exit(ft_exit(sh, cmd));

	// 2. Se não for builtin, tenta Executável Externo
	path = find_executable(cmd->argv[0], sh->env_list);
	if (!path)
	{
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(NFOUND);
	}
	env_arr = env_to_array(sh->env_list);
	execve(path, cmd->argv, env_arr);

	// 3. Se execve falhar
	perror("execve failed");
	free(path);
	free_array(env_arr);
	exit(1);
}

/*
Configura os pipes e redirecionamentos dentro do processo FILHO.
fd_in:	Descritor de leitura que veio do comando anterior (ou -1 se for o primeiro)
fd_out:	Descritor ed escrita para o próximo comando (ou -1 se for o último)
*/
static void	child_process(t_shell *sh, t_cmd *cmd, int fd_in, int fd_out)
{
	// 1. Conecta a ENTRADA (se houver pipe anterior)
	if (fd_in != -1)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}

	// 2. Conecta a SAÍDA (se houver próximo pipe)
	if (fd_out != -1)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}

	// 3. Aplica redirecionamentos de arquivos (<, >, >>), eles têm priridade sobre os pipes
	if (setup_redirects(cmd) != 0)
		exit(1);

	// 4. Executa
	exec_simple_cmd(sh, cmd);
}

/*
Gerencia o loop de execução dos comandos.
Itera sobre a lista t_cmd, criando pipes e filhos.
*/
void	execute_pipeline(t_shell *sh, t_cmd *cmds)
{
	int		pipefd[2];
	int		fd_in;		// Guarda o lado de leitura do pipe anterior
	pid_t	pid;
	t_cmd	*tmp;

	fd_in = -1;
	tmp = cmds;
	while(tmp)
	{
		// Se não for o último, cria um pipe para conectar com o próximo
		if (tmp->next)
		{
			if (pipe(pipefd) == -1)
			{
				perror("pipe");
				return ;
			}
		}
		pid = fork();
		// FILHO
		if (pid == 0)
		{
			// Se tiver próximo, a saída é pipefd[1]. Se não, é -1 (STDOUT padrão)
			if (tmp->next)
			{
				close(pipefd[0]);	// Filho não lê desse pipe, só escreve
				child_process(sh, tmp, fd_in, pipefd[1]);
			}
			else
				child_process(sh, tmp, fd_in, -1);
		}
		// PAI
		// Fecha o pipe de escrita atual (o filho já tem cópia)
		if (tmp->next)
			close(pipefd[1]);

		// Fecha o pipe de leitura ANTERIOR (já passamos pro filho atual)
		if (fd_in != -1)
			close(fd_in);

		// O input do próximo comando será a leitura do pipe atual
		if (tmp->next)
			fd_in = pipefd[0];

		tmp = tmp->next;
	}
	// Espera todos os filhos terminarem
	// O status do último comando é o que importa para o $?
	// TODO: (Implementaremos waitpid loop melhor depois, por hora wait simples)
	while (waitpid(-1, &sh->last_status, 0) > 0)
		;
	if (WIFEXITED(sh->last_status))
		sh->last_status = WEXITSTATUS(sh->last_status);
	// DUVIDA: o que sao WIFEXITED e WEXITSTATUS?
}

