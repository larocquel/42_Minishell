/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 18:27:23 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/21 14:03:34 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Cria um novo token copiando a string [start, start+len).
Usado para WORD simples e para operadores com strings fixas.
*/
static t_token	*token_new(t_type type, const char *start, size_t len)
{
	t_token	*tok;
	size_t	i;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->value = malloc(len + 1);
	if (!tok->value)
	{
		free(tok);
		return (NULL);
	}
	i = 0;
	while (i < len)
	{
		tok->value[i] = start[i];
		i++;
	}
	tok->value[len] = '\0';
	tok->type = type;
	tok->next = NULL;
	return (tok);
}

/*
Cria um token assumindo que value já está alocado
(usado no caso das palavras com aspas, onde montamos o buffer antes).
*/
static t_token	*token_new_owned(t_type type, char *value)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
	{
		free(value);
		return (NULL);
	}
	tok->type = type;
	tok->value = value;
	tok->next = NULL;
	return (tok);
}

//TO-DO: ACHO QUE PODERIAMOS UTILIZAR AS FUNCOES LST DO LIBFT
/*
Adiciona um token ao final da lista
*/
static int	token_add_back(t_token **lst, t_token *new_tok)
{
	t_token	*tmp;

	if (!new_tok)
		return (0);
	if (!*lst)
	{
		*lst = new_tok;
		return (1);
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_tok;
	return (1);
}

/*
Libera a lista de tokens inteira
*/
void	free_tokens(t_token *lst)
{
	t_token	*next;

	while (lst)
	{
		next = lst->next;
		free(lst->value);
		free(lst);
		lst = next;
	}
}

/*
Reconhece operadores de redirections/pipes e avança o índice.
IMPORTANTE: só deve ser chamado quando line[i] é |, < ou > fora de aspas.
*/
static t_type	read_operator(const char *s, size_t *i)
{
	if (s[*i] == '|')
	{
		(*i)++;
		return (PIPE);
	}
	else if (s[*i] == '>')
	{
		if (s[*i + 1] == '>')
		{
			*i += 2;
			return (R_APP);
		}
		(*i)++;
		return (R_OUT);
	}
	else if (s[*i] == '<')
	{
		if (s[*i + 1] == '<')
		{
			*i += 2;
			return (R_HDC);
		}
		(*i)++;
		return (R_IN);
	}
	// não deveria cair aqui
	(*i)++;
	return (WORD);
}

/*
Cria um token de operador com string fixa ("|", "<", ">", ">>", "<<").
*/
static t_token	*token_new_op(t_type type)
{
	if (type == PIPE)
		return (token_new(type, "|", 1));
	if (type == R_IN)
		return (token_new(type, "<", 1));
	if (type == R_OUT)
		return (token_new(type, ">", 1));
	if (type == R_APP)
		return (token_new(type, ">>", 2));
	if (type == R_HDC)
		return (token_new(type, "<<", 2));
	return (NULL);
}

/*
Lê um WORD a partir de line[*i], respeitando aspas simples e duplas.
Regras:
- Espaço/tab e | < > só quebram a palavra se estivermos FORA de aspas.
- Aspas não entram no valor final do token.
- Se terminar com aspas não fechadas, retornamos NULL (erro de sintaxe).
*/
static t_token	*read_word(const char *line, size_t *i)
{
	size_t	j;
	size_t	end;
	size_t	len;
	int		in_single;
	int		in_double;
	char	*buf;
	size_t	k;

	j = *i;
	len = 0;
	in_single = 0;
	in_double = 0;

	/* PRIMEIRA PASSAGEM: conta quantos chars vão pro token (sem aspas) */
	while (line[j] != '\0')
	{
		// fora de aspas, espaço/operador encerra a palavra
		if (!in_single && !in_double
			&& (line[j] == ' ' || line[j] == '\t'
				|| line[j] == '|' || line[j] == '<' || line[j] == '>'))
			break;
		// trata aspas simples
		if (line[j] == '\'' && !in_double)
		{
			in_single = !in_single;
			j++;
			continue;
		}
		// trata aspas duplas
		if (line[j] == '\"' && !in_single)
		{
			in_double = !in_double;
			j++;
			continue;
		}
		// caractere normal conta pro tamanho
		len++;
		j++;
	}
	// se alguma aspa ficou aberta, erro de sintaxe
	if (in_single || in_double)
		return (NULL);
	end = j;

	/* SEGUNDA PASSAGEM: copia pro buffer, pulando aspas */
	buf = malloc(len + 1);
	if (!buf)
		return (NULL);
	j = *i;
	k = 0;
	in_single = 0;
	in_double = 0;
	while (j < end)
	{
		if (line[j] == '\'' && !in_double)
		{
			in_single = !in_single;
			j++;
			continue;
		}
		if (line[j] == '\"' && !in_single)
		{
			in_double = !in_double;
			j++;
			continue;
		}
		buf[k++] = line[j++];
	}
	buf[k] = '\0';
	*i = end;
	return (token_new_owned(WORD, buf));
}

/*
Lexer:
- Ignora espaços
- Reconhece operadores: |, <, >, >>, <<
- Agrupa palavras, respeitando aspas (palavras podem conter espaços/operadores dentro)
*/
t_token	*tokenize_line(const char *line)
{
	size_t		i;
	t_token		*list;
	t_token		*tok;
	t_type	type;

	list = NULL;
	i = 0;
	while (line[i] != '\0')
	{
		// pular espaços
		if (line[i] == ' ' || line[i] == '\t')
		{
			i++;
			continue;
		}
		// operadores fora de aspas
		if (line[i] == '|' || line[i] == '<' || line[i] == '>')
		{
			type = read_operator(line, &i);
			tok = token_new_op(type);
		}
		// palavra (com possível aspas)
		else
			tok = read_word(line, &i);
		if (!tok || !token_add_back(&list, tok))
		{
			free_tokens(list);
			return (NULL);
		}
	}
	return (list);
}