/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 21:50:11 by leoaguia          #+#    #+#             */
/*   Updated: 2026/01/06 16:23:55 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Verifica se o ficheiro é um heredoc temporário gerado pelo minishell.
Se for, apaga-o do disco (unlink) para não deixar lixo, mas mantém o FD aberto.
*/
static void	unlink_if_heredoc(t_redir *r)
{
	if (r->type == R_IN && !access(r->target, F_OK))
	{
		if (ft_strncmp(r->target, ".heredoc_", 9) == 0)
			unlink(r->target);
	}
}

/*
Abre ficheiros de entrada/saída normais.
Inclui a limpeza automática de ficheiros temporários (.heredoc_*).
*/
static int	open_file(t_redir *r)
{
	int	fd;

	fd = -1;
	if (r->type == R_IN)
		fd = open(r->target, O_RDONLY);
	else if (r->type == R_OUT)
		fd = open(r->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (r->type == R_APP)
		fd = open(r->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(r->target);
	}
	else
		unlink_if_heredoc(r);
	return (fd);
}

/*
Aplica o dup2 para o STDIN ou STDOUT e fecha o fd original.
*/
static void	apply_dup(int fd, int type)
{
	if (type == R_IN || type == R_HDC)
		dup2(fd, STDIN_FILENO);
	else if (type == R_OUT || type == R_APP)
		dup2(fd, STDOUT_FILENO);
	close(fd);
}

/*
Itera sobre a lista de redireções e aplica-as.
*/
int	setup_redirects(t_cmd *cmd)
{
	t_redir	*tmp;
	int		fd;

	tmp = cmd->redirs;
	while (tmp)
	{
		fd = open_file(tmp);
		if (fd == -1)
			return (1);
		apply_dup(fd, tmp->type);
		tmp = tmp->next;
	}
	return (0);
}
