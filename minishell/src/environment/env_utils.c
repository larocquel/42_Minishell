/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 21:14:47 by leoaguia          #+#    #+#             */
/*   Updated: 2026/01/01 18:59:33 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Cria um novo nó da lista t_env.
Aloca memória para a estrutura e define os valores iniciais.
*/
t_env	*env_new(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = key;
	node->value = value;
	node->next = NULL;
	return (node);
}

/*
Adiciona um nó ao final da lista encadeada.
Se a lista estiver vazia, o novo nó se torna o primeiro.
*/
void	env_add_back(t_env **lst, t_env *new_node)
{
	t_env	*cur;

	if (!lst || !new_node)
		return ;
	if (!*lst)
	{
		*lst = new_node;
		return ;
	}
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	cur->next = new_node;
}

/*
Busca e retorna o PONTEIRO para o nó que tem a chave 'key'.
Retorna NULL se não encontrar.
Usado internamente por get_env_value e env_update
*/
t_env	*env_get_node(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

/*
Remove um nó da lista baseado na chave e libera toda a memória associada.
Lida com a remoção do primeiro nó (head) e nós do meio/fim.
*/
void	env_remove_node(t_shell *sh, char *key)
{
	t_env	*cur;
	t_env	*prev;

	cur = sh->env_list;
	prev = NULL;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			// Se tem anterior, pula o atual. Se não, atual é o head.
			if (prev)
				prev->next = cur->next;
			else
				sh->env_list = cur->next;
			// Libera memória da chave, valor e do nó
			free(cur->key);
			if (cur->value)
				free(cur->value);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}