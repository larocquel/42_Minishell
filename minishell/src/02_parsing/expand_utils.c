/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lla-rocq <lla-rocq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 22:38:26 by lla-rocq          #+#    #+#             */
/*   Updated: 2026/01/05 22:53:40 by lla-rocq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Adiciona um char ao fim da string.
Retorna nova string e liberta a antiga.
*/
char	*append_char(char *str, char c)
{
	char	*new;
	size_t	len;

	if (!str)
		str = ft_strdup("");
	len = ft_strlen(str);
	new = malloc(len + 2);
	if (!new)
	{
		free(str);
		return (NULL);
	}
	ft_strlcpy(new, str, len + 1);
	new[len] = c;
	new[len + 1] = '\0';
	free(str);
	return (new);
}

/*
Adiciona uma string ao fim de outra.
Retorna nova string e liberta a base.
*/
char	*append_str(char *base, char *val)
{
	char	*new_str;

	if (!base)
		base = ft_strdup("");
	if (!val)
		return (base);
	new_str = ft_strjoin(base, val);
	free(base);
	return (new_str);
}

/*
Auxiliar: Remove aspas da string final.
Copia chars ignorando quotes externas.
*/
void	remove_quotes_final(char *str)
{
	int	i;
	int	j;
	int	qs[2];

	i = 0;
	j = 0;
	qs[0] = 0;
	qs[1] = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !qs[1])
			qs[0] = !qs[0];
		else if (str[i] == '\"' && !qs[0])
			qs[1] = !qs[1];
		else
			str[j++] = str[i];
		i++;
	}
	str[j] = '\0';
}
