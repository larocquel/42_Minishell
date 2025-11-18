/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:52:15 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/17 15:47:04 by davmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ms_split_count(char **arr)
{
	size_t	i;

	i = 0;
	if (!arr)
		return (0);
	while (arr[i])
		i++;
	return (i);
}

/*
	*criar a string completa da variável de ambiente
	*concatena tres strings em uma nova string alocada
	*devolve o ponteiro para a nova string
	*retorna NULL em caso de falha na alocação
*/

char	*ms_strjoin3(const char *a, const char *b, const char *c)
{
	size_t	la;
	size_t	lb;
	size_t	lc;
	char	*s;

	la = 0;
	lb = 0;
	lc = 0;
	if (a)
		la = ft_strlen(a);
	if (b)
		lb = ft_strlen(b);
	if (c)
		lc = ft_strlen(c);
	s = (char *)malloc(la + lb + lc + 1);
	if (!s)
		return (NULL);
	if (a)
		ft_memcpy(s, a, la);
	if (b)
		ft_memcpy(s + la, b, lb);
	if (c)
		ft_memcpy(s + la + lb, c, lc);
	s[la + lb + lc] = '\0';
	return (s);
}
