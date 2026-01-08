/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 17:52:06 by leoaguia          #+#    #+#             */
/*   Updated: 2026/01/08 17:52:20 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Conta quantos nós existem na lista de ambiente.
*/
static int	env_list_size(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

/*
Ordena o array de ponteiros usando Bubble Sort.
Compara as chaves (keys) alfabeticamente.
*/
static void	sort_env_array(t_env **arr, int size)
{
	int		i;
	int		j;
	t_env	*temp;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (ft_strcmp(arr[j]->key, arr[j + 1]->key) > 0)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

/*
Função principal chamada pelo ft_export quando sem argumentos.
1. Cria array de ponteiros.
2. Ordena.
3. Imprime no formato declare -x VAR="VAL".
4. Libera o array (mas não os nós).
*/
void	print_sorted_export(t_shell *sh)
{
	t_env	**arr;
	t_env	*tmp;
	int		count;
	int		i;

	count = env_list_size(sh->env_list);
	arr = malloc(sizeof(t_env *) * count);
	if (!arr)
		return ;
	tmp = sh->env_list;
	i = 0;
	while (tmp)
	{
		arr[i++] = tmp;
		tmp = tmp->next;
	}
	sort_env_array(arr, count);
	i = -1;
	while (++i < count)
	{
		printf("declare -x %s", arr[i]->key);
		if (arr[i]->value)
			printf("=\"%s\"", arr[i]->value);
		printf("\n");
	}
	free(arr);
}
