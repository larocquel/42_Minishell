/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 21:50:11 by leoaguia          #+#    #+#             */
/*   Updated: 2025/12/25 22:12:25 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Função auxiliar: Abre o arquivo com as flags corretas baseado no tipo
Retorno: FD do arquivo aberto ou -1 em caso de erro
*/
static int	open_file(t_redir *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == R_IN)
		fd = open(redir->target, O_RDONLY);
	else if (redir->type == R_OUT)
		fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == R_APP)
		fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);

	if (fd == -1)
		perror("");
	return (fd);
}

/*
Função auxiliar: Realiza o dup2 para conectar o arquivo ao STDIN ou STDOUT.
*/
static void	do_dup2(int fd, int target_fd)
{
	dup2(fd, target_fd);
	close(fd);	// O original não é mais necessário, pois já foi clonado
}

/*
Função Principal: Processa todas as redireções da lista.
Retorno: 0 = sucesso ou 1 = erro
*/
int	setup_redirects(t_cmd *cmd)
{
	t_redir	*tmp;
	int		fd;

	tmp = cmd->redirs;
	while (tmp)
	{
		// TODO: Por enquanto ignoramos o HEREDOC
		if (tmp->type == R_HDC)
		{
			tmp = tmp->next;
			continue ;		// DUVIDA: Como funciona o continue?
		}

		fd = open_file(tmp);
		if (fd == -1)
			return (1);	// Erro ao abrir arquivo

		// Se for Input '<', redireciona o STDIN (0)
		if (tmp->type == R_IN)
			do_dup2(fd, STDIN_FILENO);
		// Se for Output '>' ou Append '>>', redireciona STDOUT (1)
		else if (tmp->type == R_OUT || tmp->type == R_APP)
			do_dup2(fd, STDOUT_FILENO);

		tmp = tmp->next;
	}
	return (0);
}