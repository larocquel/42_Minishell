/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 22:37:36 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/05 23:12:30 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tk_is_space(int c)
{
	return (c == ' ' || c == '\t' || c == '\n'
			|| c == '\r' || c == '\v' || c == '\f');
}

int	tk_is_operator(int c)
{
	return (c == '|' || c == '<' || c == '>');
}

t_token	*tk_new(t_toktype type, char *val, unsigned char *mask)
{
	t_token	*t;

	t = (t_token *)malloc(sizeof(*t));
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

void	tk_push(t_token **head, t_token **tail, t_token *node)
{
	if (!node)
		return ;
	if (!*head)
		*head = node;
	else
		(*tail)->next = node;
	*tail = node;
}
