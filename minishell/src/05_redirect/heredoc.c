/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:26:05 by leoaguia          #+#    #+#             */
/*   Updated: 2026/01/06 16:02:27 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Gera nomes unicos para os temporarios (.heredoc_0, .heredoc_1, ...)
*/
static char	*gen_heredoc_name(void)
{
	static int	i;
	char		*num;
	char		*name;

	num = ft_itoa(i++);
	name = ft_strjoin(".heredoc_", num);
	free(num);
	return (name);
}

/*
Handler de sinal exclusivo para o processo filho do heredoc.
Sai imediatamente com 130.
*/
static void	heredoc_sig_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(130);
}

/*
Loop de leitura executado SOMENTE no processo filho.
*/
static void	read_loop_child(int fd, char *delim)
{
	char	*line;

	signal(SIGINT, heredoc_sig_handler);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putendl_fd("minishell: warning: heredoc detected EOF", 2);
			break ;
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	exit(0);
}

/*
Cria um filho para ler o heredoc.
O pai espera e verifica se o filho saiu normalmente ou via Ctrl+C.
*/
static int	exec_heredoc(t_redir *r)
{
	int		fd;
	char	*name;
	int		pid;
	int		status;

	name = gen_heredoc_name();
	fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (free(name), 1);
	setup_signals_ignore();
	pid = fork();
	if (pid == 0)
		read_loop_child(fd, r->target);
	close(fd);
	waitpid(pid, &status, 0);
	setup_signals_interactive();
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		unlink(name);
		return (free(name), 130);
	}
	free(r->target);
	r->target = name;
	r->type = R_IN;
	return (0);
}

/*
Funcao Principal: Varre comandos e resolve heredocs um a um.
*/
int	process_heredocs(t_cmd *cmds)
{
	t_redir	*r;

	while (cmds)
	{
		r = cmds->redirs;
		while (r)
		{
			if (r->type == R_HDC)
			{
				if (exec_heredoc(r) == 130)
					return (130);
			}
			r = r->next;
		}
		cmds = cmds->next;
	}
	return (0);
}
