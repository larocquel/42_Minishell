/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lla-rocq <lla-rocq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 22:49:58 by lla-rocq          #+#    #+#             */
/*   Updated: 2026/01/05 22:55:15 by lla-rocq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Adiciona redirecionamento à lista do comando.
*/
int	redir_add_back(t_redir **lst, t_redir *new_r)
{
	t_redir	*tmp;

	if (!new_r)
		return (0);
	if (!*lst)
	{
		*lst = new_r;
		return (1);
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_r;
	return (1);
}

/*
Cria novo node de redirecionamento.
*/
t_redir	*redir_new(t_type type, const char *target)
{
	t_redir	*r;

	r = malloc(sizeof(t_redir));
	if (!r)
		return (NULL);
	r->type = type;
	r->target = ft_strdup(target);
	if (!r->target)
	{
		free(r);
		return (NULL);
	}
	r->next = NULL;
	return (r);
}

/*
Conta palavras até o próximo pipe.
*/
size_t	count_words_until_pipe(t_token *tok)
{
	size_t	count;

	count = 0;
	while (tok && tok->type != PIPE)
	{
		if (tok->type == WORD)
			count++;
		else if (tok->type != PIPE)
		{
			if (tok->next)
				tok = tok->next;
			else
				break ;
		}
		tok = tok->next;
	}
	return (count);
}

/*
Verifica se o token atual é redirect invalido (sem argumento).
*/
static int	is_invalid_redir(t_token *tok)
{
	if (tok->type == WORD || tok->type == PIPE)
		return (0);
	if (!tok->next || tok->next->type != WORD)
		return (1);
	return (0);
}

/*
Valida sintaxe geral (pipes duplicados, redirs vazios).
*/
int	validate_syntax(t_token *tok)
{
	t_token	*prev;

	prev = NULL;
	if (tok && tok->type == PIPE)
		return (0);
	while (tok)
	{
		if (tok->type == PIPE && (!prev || prev->type == PIPE))
			return (0);
		if (is_invalid_redir(tok))
			return (0);
		prev = tok;
		tok = tok->next;
	}
	if (prev && prev->type == PIPE)
		return (0);
	return (1);
}
