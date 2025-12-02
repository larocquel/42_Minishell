/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 18:43:30 by leoaguia          #+#    #+#             */
/*   Updated: 2025/12/01 14:56:29 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Função auxiliar para criar um novo nó de ambiente
static t_env	*env_new(char *key, char *value)
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

// Adiciona ao final da lista
static void		env_add_back(t_env **lst, t_env *new)
{
	t_env	*cur;

	if (!lst || !new)
		return ;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

// Recebe "KEY=VALUE", separa e cria um nó
// Se não tiver '=', o value é NULL (bash suporta vars sem valor, mas no envp geralmente tem)
static void		parse_and_add_env(t_shell *sh, char *env_str)
{
	int		i;
	char	*key;
	char	*value;

	i = 0;

	// Procura o '='
	while (env_str[i] && env_str[i] != '=')
		i++;

	// Copia a chave até o '='
	key = ft_substr(env_str, 0, i);

	// Se encontrou '=', copia o resto como valor. Senão, valor é NULL.
	if (env_str[i] == '=')
		value = ft_strdup(env_str + i + 1);
	else
		value = NULL;	// Ou ft_strdup("") dependendo de como quiser tratar
	env_add_back(&sh->env_list, env_new(key, value));
}

// Função principal chamada no main
void	init_env(t_shell *sh, char **envp)
{
	int	i;

	sh->env_list = NULL;
	i = 0;
	while (envp[i])
	{
		parse_and_add_env(sh, envp[i]);
		i++;
	}
}

// Função utilitária para buscar valor (usaremos na expansão $)
char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}