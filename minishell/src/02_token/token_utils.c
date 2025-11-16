/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 22:37:36 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/16 19:25:05 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Retorna 1 se o caractere for espaço em branco (space/tab/newline/etc.).
*/
int	tk_is_space(int c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\r' || c == '\v' || c == '\f');
}

/*
** Retorna 1 se o caractere for um operador do shell simples: | < >
*/
int	tk_is_operator(int c)
{
	return (c == '|' || c == '<' || c == '>');
}

/*
** Cria um novo token. Assume a posse de 'val' e 'mask' (podem ser NULL).
** Em falha, libera 'val' e 'mask' e retorna NULL.
*/
t_tk	*tk_new(t_tktype type, char *val, char *mask)
{
	t_tk	*t;

	t = (t_tk *)malloc(sizeof(*t));
	if (!t)
	{
		free(val);
		free(mask);
		return (NULL);
	}
	t->type = type;
	t->val = val;
	t->qmask = mask;
	t->next = NULL;
	return (t);
}

/*
** Enfileira 'node' no final da lista indicada por (head, tail).
*/
void	tk_push(t_tk **head, t_tk **tail, t_tk *node)
{
	if (!node)
		return ;
	if (!*head)
		*head = node;
	else
		(*tail)->next = node;
	*tail = node;
}
