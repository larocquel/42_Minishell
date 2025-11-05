/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_buf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 22:44:47 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/05 23:35:28 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	tk_grow_str(t_str *b, int *err)
{
	char	*nb;
	size_t	nc;
	size_t	i;

	if (b->len + 1 < b->cap)
		return (1);
	if (b->cap == 0)
		nc = 16;
	else
		nc = b->cap * 2;
	nb = (char *)malloc(nc);
	if (!nb)
		return (*err = 1, 0);
	i = 0;
	while (i < b->len)
	{
		nb[i] = b->data[i];
		i++;
	}
	free(b->data);
	b->data = nb;
	b->cap = nc;
	return (1);
}

char	*tk_add_str(t_str *b, char c, int *err)
{
	if (!tk_grow_str(b, err))
		return (NULL);
	b->data[b->len] = c;
	b->len++;
	return (b->data);
}

static int	tk_grow_mask(t_mask *m, int *err)
{
	unsigned char	*nb;
	size_t			nc;
	size_t			i;

	if (m->len + 1 < m->cap)
		return (1);
	if (m->cap == 0)
		nc = 16;
	else
		nc = m->cap * 2;
	nb = (unsigned char *)malloc(nc);
	if (!nb)
		return (*err = 1, 0);
	i = 0;
	while (i < m->len)
	{
		nb[i] = m->data[i];
		i++;
	}
	free(m->data);
	m->data = nb;
	m->cap = nc;
	return (1);
}

unsigned char	*tk_add_mask(t_mask *m, unsigned char v, int *err)
{
	if (!tk_grow_mask(m, err))
		return (NULL);
	m->data[m->len] = v;
	m->len++;
	return (m->data);
}