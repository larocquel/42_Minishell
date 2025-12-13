/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 18:43:30 by leoaguia          #+#    #+#             */
/*   Updated: 2025/12/09 21:56:24 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Auxiliar estática: Recebe string crua "KEY=VAL", separa e adiciona na lista.
Mantém-se estática pois só é usada pelo init_env.
*/
static void	parse_and_add_env(t_shell *sh, char *env_str)
{
	int		i;
	char	*key;
	char	*value;

	i = 0;
	while (env_str[i] && env_str[i] != '=')
		i++;
	key = ft_substr(env_str, 0, i);
	if (env_str[i] == '=')
		value = ft_strdup(env_str + i + 1);
	else
		value = NULL;
	env_add_back(&sh->env_list, env_new(key, value));
}

/*
Inicializa o ambiente copiando o envp do sistema.
*/
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

/*
Retorna o VALOR de uma variável (string).
Agora reutiliza env_get_node para evitar duplicação de lógica.
*/
char	*get_env_value(t_env *env, char *key)
{
	t_env	*node;

	node = env_get_node(env, key);
	if (node)
		return (node->value);
	return (NULL);
}

/*
Lógica do EXPORT:
- Se a variável já existe: Atualiza o valor (se value não for NULL).
- Se não existe: Cria um novo nó.
*/
void	env_update(t_shell *sh, char *key, char *value)
{
	t_env	*node;
	char	*dup_val;

	node = env_get_node(sh->env_list, key);
	if (node)
	{
		// Se a variável existe e passamos um novo valor, atualiza.
		// Ex: export VAR=novo (atualiza). export VAR (não muda o valor).
		if (value)
		{
			if (node->value)
				free(node->value);
			node->value = ft_strdup(value);
		}
	}
	else
	{
		// Verifica antes de duplicar
		if (value)
			dup_val = ft_strdup(value);
		else
			dup_val = NULL;
		env_add_back(&sh->env_list, env_new(ft_strdup(key), dup_val));
	}
}