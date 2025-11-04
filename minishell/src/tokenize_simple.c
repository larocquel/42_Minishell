/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_simple.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:51:59 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/03 14:04:26 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ms_is_ws(int c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	if (c == '\r' || c == '\v' || c == '\f')
		return (1);
	return (0);
}

static char	*ms_dup_range(const char *s, size_t a, size_t b)
{
	size_t	len;
	char	*out;

	len = b - a;
	out = (char *)malloc(len + 1);
	if (!out)
		return (NULL);
	memcpy(out, s + a, len);
	out[len] = '\0';
	return (out);
}

char	**tokenize_simple(const char *line)
{
	size_t	i = 0;
	size_t	k = 0;
	size_t	n = 0;
	char	**toks;

	while (line[i])
	{
		while (line[i] && ms_is_ws((unsigned char)line[i]))
			i++;
		if (!line[i])
			break ;
		n++;
		while (line[i] && !ms_is_ws((unsigned char)line[i]))
			i++;
	}
	toks = (char **)calloc(n + 1, sizeof(char *));
	if (!toks)
		return (NULL);
	i = 0;
	while (line[i])
	{
		while (line[i] && ms_is_ws((unsigned char)line[i]))
			i++;
		if (!line[i])
			break ;
		size_t	a = i;
		while (line[i] && !ms_is_ws((unsigned char)line[i]))
			i++;
		toks[k] = ms_dup_range(line, a, i);
		if (!toks[k])
			return (free_tokens(toks), NULL);
		k++;
	}
	toks[k] = NULL;
	return (toks);
}

void	free_tokens(char **toks)
{
	size_t	i = 0;

	if (!toks)
		return ;
	while (toks[i])
	{
		free(toks[i]);
		i++;
	}
	free(toks);
}
