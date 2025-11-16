/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:52:05 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/16 19:20:32 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ms_env_dup(char **envp)
{
	size_t	n;
	size_t	i;
	char	**dup;

	n = 0;
	while (envp && envp[n]) n++;
	dup = (char **)calloc(n + 1, sizeof(char *));
	if (!dup) return (NULL);
	i = 0;
	while (i < n)
	{
		dup[i] = strdup(envp[i]);
		if (!dup[i]) return (ms_free_strarr(dup), NULL);
		i++;
	}
	dup[n] = NULL;
	return (dup);
}

void	ms_free_strarr(char **arr)
{
	size_t	i;

	if (!arr) return ;
	i = 0;
	while (arr[i]) { free(arr[i]); i++; }
	free(arr);
}

char	*ms_getenv(char **envp, const char *key)
{
	size_t	klen;
	size_t	i;

	klen = strlen(key);
	i = 0;
	while (envp && envp[i])
	{
		if (!strncmp(envp[i], key, klen) && envp[i][klen] == '=')
			return (envp[i] + klen + 1);
		i++;
	}
	return (NULL);
}

int	ms_setenv(t_ms *sh, const char *k, const char *v)
{
	size_t	klen;
	size_t	i;
	char	*nv;
	char	**newtab;

	if (!k || !*k || strchr(k, '=')) return (-1);
	nv = ms_strjoin3(k, "=", v ? v : "");
	if (!nv) return (-1);
	klen = strlen(k);
	i = 0;
	while (sh->envp && sh->envp[i])
	{
		if (!strncmp(sh->envp[i], k, klen) && sh->envp[i][klen] == '=')
			return (free(sh->envp[i]), sh->envp[i] = nv, 0);
		i++;
	}
	newtab = (char **)realloc(sh->envp, sizeof(char *) * (i + 2));
	if (!newtab) return (free(nv), -1);
	sh->envp = newtab;
	sh->envp[i] = nv;
	sh->envp[i + 1] = NULL;
	return (0);
}

int	ms_unsetenv(t_ms *sh, const char *key)
{
	size_t	i;
	size_t	j;
	size_t	klen;
	int		f;

	i = 0;
	j = 0;
	f = 0;
	klen = strlen(key);
	while (sh->envp && sh->envp[i])
	{
		if (!strncmp(sh->envp[i], key, klen) && sh->envp[i][klen] == '=')
		{
			free(sh->envp[i]);
			f = 1;
		}
		else
			sh->envp[j++] = sh->envp[i];
		i++;
	}
	if (f) sh->envp[j] = NULL;
	return (f ? 0 : -1);
}
