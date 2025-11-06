/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_read.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 22:51:14 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/05 23:12:15 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*tk_read_op(const char *s, size_t *i)
{
	if (s[*i] == '|')
	{
		(*i)++;
		return (tk_new(PIPE, NULL, NULL));
	}
	if (s[*i] == '<')
	{
		if (s[*i + 1] == '<')
		{
			*i += 2;
			return (tk_new(HEREDOC, NULL, NULL));
		}
		(*i)++;
		return (tk_new(REDIR_IN, NULL, NULL));
	}
	if (s[*i] == '>')
	{
		if (s[*i + 1] == '>')
		{
			*i += 2;
			return (tk_new(REDIR_APP, NULL, NULL));
		}
		(*i)++;
		return (tk_new(REDIR_OUT, NULL, NULL));
	}
	return (NULL);
}

static int	tk_copy_quoted(const char *s, size_t *i, int *err, t_str *b, t_mask *m, char q)
{
	while (s[*i] && s[*i] != q)
	{
		if (!tk_add_str(b, s[*i], err))
			return (0);
		if (q == '\'')
		{
			if (!tk_add_mask(m, SQ, err))
				return (0);
		}
		else
		{
			if (!tk_add_mask(m, DQ, err))
				return (0);
		}
		(*i)++;
	}
	if (s[*i] != q)
		return (*err = 1, 0);
	(*i)++;
	return (1);
}

char	*tk_read_word(const char *s, size_t *i, int *err, unsigned char **out_mask)
{
	t_str	b;
	t_mask	m;
	char		q;

	b.data = NULL;
	b.cap = 0;
	b.len = 0;
	m.data = NULL;
	m.cap = 0;
	m.len = 0;
	while (s[*i] && !tk_is_space(s[*i]) && !tk_is_operator(s[*i]))
	{
		if (s[*i] == '\'' || s[*i] == '\"')
		{
			q = s[*i];
			(*i)++;
			if (!tk_copy_quoted(s, i, err, &b, &m, q))
			{
				free(b.data);
				free(m.data);
				return (NULL);
			}
		}
		else
		{
			if (!tk_add_str(&b, s[*i], err))
				return (free(m.data), NULL);
			if (!tk_add_mask(&m, NQ, err))
				return (free(b.data), free(m.data), NULL);
			(*i)++;
		}
	}
	if (!tk_add_str(&b, '\0', err))
		return (free(m.data), NULL);
	*out_mask = m.data;
	return (b.data);
}
