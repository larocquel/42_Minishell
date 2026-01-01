/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 21:12:31 by leoaguia          #+#    #+#             */
/*   Updated: 2026/01/01 20:27:17 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Converte nossa lista t_env de volta para char **
Formato: "KEY_VALUE"
Necessário para passar para execve()
*/
char	**env_to_array(t_env *env_list)
{
	t_env	*tmp;
	char	**arr;
	int		count;
	int		i;

	count = 0;
	tmp = env_list;
	//	1. Conta nodes
	while (tmp && ++count)
		tmp = tmp->next;
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	tmp = env_list;
	i = 0;
	while (tmp)
	{
		// 2. Juntamos: KEY + "=" + VALUE
		if (tmp->value)
			arr[i] = ft_strjoin3(tmp->key, "=", tmp->value);
		// 3. Se não tem valor, usamos o ft_strjoin normal, só com "="
		else
			arr[i] = ft_strjoin(tmp->key, "=");
		tmp = tmp->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

/*
Procura o binário do comando.
1. Se cmd começar com '/' ou './', assume que é caminho absoluto/relativo.
2. Senão, busca nas pastas de variável PATH
*/
char	*find_executable(char *cmd, t_env *env_list)
{
	char	**paths;
	char	*full_path;
	int		i;

	// 1. Caminho absoluto/relativo (ex: /bin/ls ou ./minishell)
	if (ft_strchr(cmd, '/'))
	{
		// access verifica se arquivo existe (F_OK) e é executável (X_OK)
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}

	// 2. Busca no PATH
	if (!get_env_value(env_list, "PATH"))
		return (NULL);
	paths = ft_split(get_env_value(env_list, "PATH"), ':');
	if (!paths)
		return (NULL);

	// 3. Itera sobre as pastas
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin3(paths[i], "/", cmd);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_array(paths); // 4. Limpeza se não achar nada (IMPORTANTE PARA LEAKS)
	return (NULL);
}
