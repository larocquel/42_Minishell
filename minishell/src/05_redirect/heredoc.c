/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:26:05 by leoaguia          #+#    #+#             */
/*   Updated: 2026/01/12 19:02:40 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Generates unique temporary file name.
*/
static char	*generate_heredoc_name(void)
{
	static int	i = 0;
	char		*num;
	char		*name;

	num = ft_itoa(i++);
	name = ft_strjoin("/tmp/.minishell_hd_", num);
	free(num);
	return (name);
}

/*
Readline reading loop. Returns 130 if interrupted by signal.
*/
static int	heredoc_loop(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		if (g_signal == SIGINT)
			return (130);
		line = readline("> ");
		if (g_signal == SIGINT)
		{
			if (line)
				free(line);
			return (130);
		}
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	return (0);
}

/*
Prepares the file, executes the loop and replaces the redirection target.
*/
static int	exec_heredoc(t_redir *r)
{
	int		fd;
	char	*filename;
	int		status;

	filename = generate_heredoc_name();
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("minishell: heredoc");
		free(filename);
		return (1);
	}
	status = heredoc_loop(fd, r->target);
	close(fd);
	if (status == 130)
	{
		unlink(filename);
		free(filename);
		return (130);
	}
	free(r->target);
	r->target = filename;
	r->type = R_IN;
	return (0);
}

/*
Iterates over commands and redirections processing heredocs.
*/
int	process_heredocs(t_cmd *cmds)
{
	t_cmd	*tmp;
	t_redir	*r;
	int		ret;

	tmp = cmds;
	while (tmp)
	{
		r = tmp->redirs;
		while (r)
		{
			if (r->type == R_HDC)
			{
				ret = exec_heredoc(r);
				if (ret != 0)
					return (ret);
			}
			r = r->next;
		}
		tmp = tmp->next;
	}
	return (0);
}
