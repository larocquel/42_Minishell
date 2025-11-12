/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:52:25 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/02 22:52:26 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ms_try_join(const char *a, const char *b, const char *c)
{
	char	*s;

	s = ms_strjoin3(a, b, c);
	if (!s) return (NULL);
	if (access(s, X_OK) == 0) return (s);
	free(s);
	return (NULL);
}

char	*ms_search_path(char **envp, const char *cmd)
{
	char	*path;
	char	*cpy;
	char	*tok;
	char	*p;

	if (strchr(cmd, '/')) return (strdup(cmd));
	path = ms_getenv(envp, "PATH");
	if (!path) return (strdup(cmd));
	cpy = strdup(path);
	if (!cpy) return (NULL);
	tok = strtok(cpy, ":");
	while (tok)
	{
		p = ms_try_join(tok, "/", cmd);
		if (p) return (free(cpy), p);
		tok = strtok(NULL, ":");
	}
	free(cpy);
	return (strdup(cmd));
}
