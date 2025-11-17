/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_mark.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davmendo <davmendo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:31:21 by davmendo          #+#    #+#             */
/*   Updated: 2025/11/17 14:00:41 by davmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ms_env_keycmp(const char *entry, const char *key)
{
	size_t	i;

	i = 0;
	while (entry[i] != '\0' && entry[i] != '=' && key[i] != '\0')
	{
		if ((unsigned char)entry[i] != (unsigned char)key[i])
			return ((unsigned char)entry[i] - (unsigned char)key[i]);
		i++;
	}
	if (key[i] == '\0' && (entry[i] == '\0' || entry[i] == '='))
		return (0);
	return ((unsigned char)entry[i] - (unsigned char)key[i]);
}

static int	ms_env_has_key_or_novalue(char **envp, const char *key)
{
	size_t	i;

	if (envp == NULL || key == NULL)
		return (0);
	i = 0;
	while (envp[i] != NULL)
	{
		if (ms_env_keycmp(envp[i], key) == 0)
			return (1);
		i++;
	}
	return (0);
}

/*
 * export VAR
 * - se KEY já existir (KEY ou KEY=VAL), não faz nada.
 * - Se não existir, adiciona "KEY" (sem '=') ao envp.
 */
int	ms_export_mark_no_value(t_shell *sh, const char *key)
{
	size_t	i;
	char	*nv;
	char	**newtab;

	if (sh == NULL || key == NULL)
		return (-1);
	if (ms_env_has_key_or_novalue(sh->envp, key))
		return (0);
	nv = ft_strdup(key);
	if (nv == NULL)
		return (-1);
	i = 0;
	while (sh->envp != NULL && sh->envp[i] != NULL)
		i++;
	newtab = (char **)realloc(sh->envp, sizeof(char *) * (i + 2));
	if (newtab == NULL)
	{
		free(nv);
		return (-1);
	}
	sh->envp = newtab;
	sh->envp[i] = nv;
	sh->envp[i + 1] = NULL;
	return (0);
}
