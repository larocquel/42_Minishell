/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:52:22 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/02 22:52:23 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(const char *cmd)
{
	if (!strcmp(cmd, "echo")) return (1);
	if (!strcmp(cmd, "pwd")) return (1);
	if (!strcmp(cmd, "env")) return (1);
	if (!strcmp(cmd, "exit")) return (1);
	if (!strcmp(cmd, "cd")) return (1);
	if (!strcmp(cmd, "export")) return (1);
	if (!strcmp(cmd, "unset")) return (1);
	return (0);
}

int	exec_external(t_shell *sh, char **argv)
{
	pid_t	pid;
	int		st;
	char	*prog;

	prog = ms_search_path(sh->envp, argv[0]);
	if (!prog) return (127);
	pid = fork();
	if (pid < 0) return (perror("fork"), free(prog), 1);
	if (pid == 0)
	{
		signals_setup_child();
		execve(prog, argv, sh->envp);
		fprintf(stderr, "minishell: %s: %s\n", prog, strerror(errno));
		_exit(errno == ENOENT ? 127 : 126);
	}
	free(prog);
	if (waitpid(pid, &st, 0) < 0) return (1);
	if (WIFEXITED(st)) return (WEXITSTATUS(st));
	if (WIFSIGNALED(st)) return (128 + WTERMSIG(st));
	return (1);
}
