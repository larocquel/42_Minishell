/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:26:05 by leoaguia          #+#    #+#             */
/*   Updated: 2026/01/19 21:11:14 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Expands variables ($VAR and $?) inside a heredoc line.
*/
static char	*expand_hd_line(t_shell *sh, char *line)
{
	char	*res;
	char	*key;
	char	*val;
	int		i;
	int		start;

	res = ft_strdup("");
	i = 0;
	while (line[i])
	{
		if (line[i] == '$'
			&& (ft_isalnum(line[i + 1])
				|| line[i + 1] == '_' || line[i + 1] == '?'))
		{
			i++;
			start = i;
			if (line[i] == '?')
			{
				val = ft_itoa(sh->last_status);
				i++;
			}
			else
			{
				while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
					i++;
				key = ft_substr(line, start, i - start);
				val = get_env_value(sh->env_list, key);
				if (val)
					val = ft_strdup(val);
				else
					val = ft_strdup("");
				free(key);
			}
			res = append_str(res, val);
			free(val);
		}
		else
			res = append_char(res, line[i++]);
	}
	free(line);
	return (res);
}

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
static int	heredoc_loop(t_shell *sh, int fd, char *delimiter, int expand)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (g_signal == 130)
				return (130);
			ft_putendl_fd("minishell: warning: heredoc delimited by EOF", 2);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (expand && ft_strchr(line, '$'))
			line = expand_hd_line(sh, line);
		ft_putendl_fd(line, fd);
		free(line);
	}
	return (0);
}

/*
Prepares the file, checks quotes on delimiter, executes loop.
*/
static int	exec_heredoc(t_shell *sh, t_redir *r)
{
	int		fd;
	int		stdin_backup;
	char	*filename;
	int		status;
	int		do_expand;

	filename = generate_heredoc_name();
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (free(filename), 1);
	do_expand = 1;
	if (ft_strchr(r->target, '\'') || ft_strchr(r->target, '\"'))
	{
		do_expand = 0;
		remove_quotes_final(r->target);
	}
	stdin_backup = dup(STDIN_FILENO);
	setup_signals_heredoc();
	status = heredoc_loop(sh, fd, r->target, do_expand);
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
	setup_signals_interactive();
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
int	process_heredocs(t_shell *sh, t_cmd *cmds)
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
				ret = exec_heredoc(sh, r);
				if (ret != 0)
					return (ret);
			}
			r = r->next;
		}
		tmp = tmp->next;
	}
	return (0);
}
