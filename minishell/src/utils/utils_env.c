/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:52:05 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/18 14:57:51 by davmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * envp é um array de strings terminado em NULL 
 * ex: envp[0] = "PATH=/usr/bin:/bin";
 * criar uma copia indepedente do envp
 * duplica o array de ponteiros (char **)
 * duplica tambem cada string ("PATH=...", "HOME=...")
 * size_t n = numero de variaveis de ambiente
*/

char	**ms_env_dup(char **envp)
{
	size_t	n;
	size_t	i;
	char	**dup;

	n = 0;
	while (envp && envp[n])
		n++;
	dup = (char **)calloc(n + 1, sizeof(char *));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < n)
	{
		dup[i] = strdup(envp[i]);
		if (!dup[i])
			return (ms_free_strarr(dup), NULL);
		i++;
	}
	dup[n] = NULL;
	return (dup);
}

/*
 * free da memória um array de strings (char **) terminado NULL
*/

void	ms_free_strarr(char **arr)
{
	size_t	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

/*
 * procurar dentro do envp a variavel KEY
 * devolver apenas o valor dessa variavel (depois do =)
 * ex: ms_getenv(envp, "PATH") deve devolver "/usr/bin:/bin"
 * se não existir devolve NULL
*/

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
