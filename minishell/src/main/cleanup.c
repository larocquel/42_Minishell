/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 16:18:21 by leoaguia          #+#    #+#             */
/*   Updated: 2026/01/01 19:15:46 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Libera a lista de tokens.
Percorre a lista e libera a string 'value' e o próprio nó 't_token'.
*/
void	free_tokens(t_token *tokens)
{
	t_token	*next;

	while (tokens)
	{
		next = tokens->next;
		if (tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = next;
	}
}

/*
Libera um array de strings (char **)
Libera cada string individualmente e depois o array de ponteiros.
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
Libera a lista de redirecionamentos (t_redir).
Percorre a lista e libera a string 'target' e o nó 't_redir'.
*/
void	free_redirs(t_redir *r)
{
	t_redir	*next;

	while (r)
	{
		next = r->next;
		if (r->target)
			free(r->target);
		free(r);
		r = next;
	}
}

/*
Libera a lista de comandos (t_cmd).
Percorre a lista e libera o array 'argv', a lista de redirecionamentos 'redirs' e o nó 't_cmd'.
*/
void    free_cmds(t_cmd *cmds)
{
    t_cmd   *tmp;

    while (cmds)
    {
        tmp = cmds->next;
        if (cmds->argv)
            free_array(cmds->argv);
        if (cmds->redirs)
            free_redirs(cmds->redirs);
        free(cmds);
        cmds = tmp;
    }
}

/*
Libera a lista de variáveis de ambiente (t_env).
Percorre a lista e libera as strings 'key' e 'value', e o nó 't_env'.
*/
void    free_env_list(t_env *env)
{
    t_env   *tmp;

    while (env)
    {
        tmp = env->next;
        if (env->key)
            free(env->key);
        if (env->value)
            free(env->value);
        free(env);
        env = tmp;
    }
}