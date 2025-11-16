/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:52:15 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/02 22:52:16 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ms_split_count(char **arr)
{
	size_t	i;

	i = 0;
	if (!arr) return (0);
	while (arr[i]) i++;
	return (i);
}

char	*ms_strjoin3(const char *a, const char *b, const char *c)
{
	size_t	la;
	size_t	lb;
	size_t	lc;
	char	*s;

	la = strlen(a ? a : "");
	lb = strlen(b ? b : "");
	lc = strlen(c ? c : "");
	s = (char *)malloc(la + lb + lc + 1);
	if (!s) return (NULL);
	memcpy(s, a, la);
	memcpy(s + la, b, lb);
	memcpy(s + la + lb, c, lc);
	s[la + lb + lc] = '\0';
	return (s);
}
