/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 21:12:31 by leoaguia          #+#    #+#             */
/*   Updated: 2025/12/07 20:15:47 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Libera um array de strings (char **)
Útil para limpar o resultado de ft_split ou o env array
*/
void	free_array(char **arr)
{
	int	i;

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

	// Se for caminho absoluto/relativo, checa direto
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0) // DUVIDA: O que é access?
			return (ft_strdup(cmd));
		return (NULL);
	}
	// Busca o PATH no environment e divide
	// Nota: Se get_env_value retornar NULL, ft_split pode quebrar se não proteger.
	// Vamos assumir que get_env_value retorna NULL se não achar.
	if (!get_env_value(env_list, "PATH"))
		return (NULL);
	paths = ft_split(get_env_value(env_list, "PATH"), ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[i++])
	{
		// Monta: "/usr/bin" + "/" + "ls" em uma tacada só
		full_path = ft_strjoin3(paths[i], "/", cmd);
		if (access(full_path, F_OK | X_OK) == 0)
			return (free_array(paths), full_path);
		free(full_path);
	}
	free_array(paths);
	return (NULL);
}
