/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 14:17:00 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/16 19:27:31 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Lê o próximo token a partir de s[*i].
** - Se operador: delega a tk_read_op.
** - Senão: lê WORD com tk_read_word, cria token WORD via tk_new.
** Em erro de alocação/aspas: retorna NULL (err já ajustado por tk_read_word).
*/
static t_tk	*lex_next_token(const char *s, size_t *i, int *err)
{
	char			*val;
	char			*mask;
	t_tk			*node;

	if (tk_is_operator(s[*i]))
		return (tk_read_op(s, i));
	val = tk_read_word(s, i, err, &mask);
	if (*err || !val)
		return (NULL);
	node = tk_new(WORD, val, mask);
	if (!node)
	{
		free(val);
		free(mask);
		*err = 1;
		return (NULL);
	}
	return (node);
}

/*
** Varre a linha:
** - pula espaços
** - cria tokens (operadores ou WORD) e encadeia em lista
** - em erro: libera a lista e retorna NULL
*/
t_tk	*lex_line(const char *s, int *err)
{
	size_t	i;
	t_tk	*head;
	t_tk	*tail;
	t_tk	*node;

	if (!s || !err)
		return (NULL);
	*err = 0;
	head = NULL;
	tail = NULL;
	i = 0;
	while (s[i])
	{
		while (s[i] && tk_is_space(s[i]))
			i++;
		if (!s[i])
			break ;
		node = lex_next_token(s, &i, err);
		if (!node)
			return (free_token_list(head), NULL);
		tk_push(&head, &tail, node);
	}
	return (head);
}

/*
** Libera toda a lista de tokens (val, qmask e os nós).
*/
void	free_token_list(t_tk *lst)
{
	t_tk	*n;

	while (lst)
	{
		n = lst->next;
		free(lst->val);
		free(lst->qmask);
		free(lst);
		lst = n;
	}
}

/*
** Libera um vetor NULL-terminated de strings (args).
*/
void	free_tokens(char **toks)
{
	size_t	i;

	if (!toks)
		return ;
	i = 0;
	while (toks[i])
	{
		free(toks[i]);
		i++;
	}
	free(toks);
}
