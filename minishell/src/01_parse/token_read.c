/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_read.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 22:51:14 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/12 23:21:37 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_read.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 22:51:14 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/12 23:40:00 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Lê |, <, <<, >, >> a partir de s[*i], avança o índice e cria o token.
*/
t_token	*tk_read_op(const char *s, size_t *i)
{
	if (s[*i] == '|')
		return ((*i)++, tk_new(PIPE, NULL, NULL));
	if (s[*i] == '<')
	{
		if (s[*i + 1] == '<')
			return (*i += 2, tk_new(HEREDOC, NULL, NULL));
		return ((*i)++, tk_new(REDIR_IN, NULL, NULL));
	}
	if (s[*i] == '>')
	{
		if (s[*i + 1] == '>')
			return (*i += 2, tk_new(REDIR_APP, NULL, NULL));
		return ((*i)++, tk_new(REDIR_OUT, NULL, NULL));
	}
	return (NULL);
}

/*
** Copia conteúdo entre aspas q (' ou ") sem as aspas; marca SQ/DQ na máscara.
** Se não encontrar fechamento, seta *err=1 e retorna 0.
*/
static int	tk_copy_quoted(t_reader *r, t_acc *a, char q)
{
	while (r->s[*r->i] && r->s[*r->i] != q)
	{
		if (!tk_add_str(a->b, r->s[*r->i], r->err))
			return (0);
		if (q == '\'')
		{
			if (!tk_add_mask(a->m, SQ, r->err))
				return (0);
		}
		else
		{
			if (!tk_add_mask(a->m, DQ, r->err))
				return (0);
		}
		(*r->i)++;
	}
	if (r->s[*r->i] != q)
	{
		*(r->err) = 1;
		return (0);
	}
	(*r->i)++;
	return (1);
}

/*
** Preenche b/m lendo até espaço, operador ou fim: trata aspas e texto simples.
*/
static int	tk_word_fill(t_reader *r, t_acc *a)
{
	char	q;

	while (r->s[*r->i] && !tk_is_space(r->s[*r->i])
		&& !tk_is_operator(r->s[*r->i]))
	{
		if (r->s[*r->i] == '\'' || r->s[*r->i] == '\"')
		{
			q = r->s[*r->i];
			(*r->i)++;
			if (!tk_copy_quoted(r, a, q))
				return (0);
		}
		else
		{
			if (!tk_add_str(a->b, r->s[*r->i], r->err))
				return (0);
			if (!tk_add_mask(a->m, NQ, r->err))
				return (0);
			(*r->i)++;
		}
	}
	return (1);
}

/*
** Zera buffers de texto e máscara.
*/
static void	tk_word_init(t_str *b, t_mask *m)
{
	b->data = NULL;
	b->cap = 0;
	b->len = 0;
	m->data = NULL;
	m->cap = 0;
	m->len = 0;
}

/*
** Lê um WORD removendo aspas. Em aspas não fechadas: *err=1 e retorna NULL.
** Retorna a string em b.data e a máscara em *out_mask (mesmo tamanho).
*/
char	*tk_read_word(
	const char *s, size_t *i, int *err, unsigned char **out_mask)
{
	t_reader	r;
	t_str		b;
	t_mask		m;
	t_acc		a;

	r.s = s;
	r.i = i;
	r.err = err;
	tk_word_init(&b, &m);
	a.b = &b;
	a.m = &m;
	if (!tk_word_fill(&r, &a))
	{
		free(b.data);
		free(m.data);
		return (NULL);
	}
	if (!tk_add_str(&b, '\0', err))
	{
		free(m.data);
		return (NULL);
	}
	*out_mask = m.data;
	return (b.data);
}
