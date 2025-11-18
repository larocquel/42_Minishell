/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manage.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davmendo <davmendo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 16:13:30 by davmendo          #+#    #+#             */
/*   Updated: 2025/11/18 15:25:41 by davmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * atualiza uma variavel de ambiente que ja existe
 * se a variavel ja existe > atualiza com ms_update_env(...)
 * se nao existe (return 0)
 * const char *k = a KEY, nome da variavel: "PATH", "HOME"...
 * size_t klen = tamanho da key
 * char *nv = a nova string completa "KEY=VALOR"
*/

static int	ms_update_env(t_shell *sh, const char *k, size_t klen, char *nv)
{
	size_t	i;

	i = 0;
	while (sh->envp && sh->envp[i])
	{
		if (!ft_strncmp(sh->envp[i], k, klen) \
			&& sh->envp[i][klen] == '=')
		{
			free(sh->envp[i]);
			sh->envp[i] = nv;
			return (1);
		}
		i++;
	}
	return (0);
}

/*
 * adiciona uma variavel nova ao env
 * realocando o array para caber mais um elemento
*/

static int	ms_add_new_env(t_shell *sh, char *nv, size_t i)
{
	char	**newtab;

	newtab = (char **)realloc(sh->envp, sizeof(char *) * (i + 2));
	if (!newtab)
	{
		free(nv);
		return (-1);
	}
	sh->envp = newtab;
	sh->envp[i] = nv;
	sh->envp[i + 1] = NULL;
	return (0);
}

/*
 * garante que a variavel do ambiente 'k' passa a ter o valor 'v'
 * atualiza se ja existe ou cria se nao existir
 * const char *k = KEY o nome da variavel: "PATH", "HOME"...
 * const char *v = value o valor: "/usr/bin:/bin"...
 */

int	ms_setenv(t_shell *sh, const char *k, const char *v)
{
	size_t		klen;
	size_t		i;
	char		*nv;
	const char	*val;

	if (!k || !*k || ft_strchr(k, '='))
		return (-1);
	val = "";
	if (v)
		val = v;
	nv = ms_strjoin3(k, "=", val);
	if (!nv)
		return (-1);
	klen = ft_strlen(k);
	if (ms_update_env(sh, k, klen, nv))
		return (0);
	i = 0;
	while (sh->envp && sh->envp[i])
		i++;
	return (ms_add_new_env(sh, nv, i));
}

int	ms_unsetenv(t_shell *sh, const char *key)
{
	size_t	i;
	size_t	j;
	size_t	klen;
	int		removed;

	i = 0;
	j = 0;
	removed = 0;
	klen = ft_strlen(key);
	while (sh->envp && sh->envp[i])
	{
		if (!ft_strncmp(sh->envp[i], key, klen) && sh->envp[i][klen] == '=')
		{
			free(sh->envp[i]);
			removed = 1;
		}
		else
			sh->envp[j++] = sh->envp[i];
		i++;
	}
	if (removed)
		sh->envp[j] = NULL;
	if (removed)
		return (0);
	return (-1);
}
