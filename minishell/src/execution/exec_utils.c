/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 21:12:31 by leoaguia          #+#    #+#             */
/*   Updated: 2026/01/05 18:13:46 by leoaguia         ###   ########.fr       */
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
	while (tmp && ++count)
		tmp = tmp->next;
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	tmp = env_list;
	i = 0;
	while (tmp)
	{
		if (tmp->value)
			arr[i] = ft_strjoin3(tmp->key, "=", tmp->value);
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
1. Se cmd começar com '/' ou './', assume caminho absoluto/relativo.
2. Senão, busca nas pastas de variável PATH
*/
char	*find_executable(char *cmd, t_env *env_list)
{
	char	**paths;
	char	*full_path;
	int		i;

	if (ft_strchr(cmd, '/') && access(cmd, F_OK | X_OK) == 0)
		return (ft_strdup(cmd));
	if (!get_env_value(env_list, "PATH"))
		return (NULL);
	paths = ft_split(get_env_value(env_list, "PATH"), ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		full_path = ft_strjoin3(paths[i], "/", cmd);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			free_array(paths);
			return (full_path);
		}
		free(full_path);
	}
	free_array(paths);
	return (NULL);
}
