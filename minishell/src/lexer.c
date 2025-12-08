/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 18:27:23 by leoaguia          #+#    #+#             */
/*   Updated: 2025/12/08 19:25:55 by leoaguia         ###   ########.fr       */
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
TODO: Correto ou necessário adaptar para o novo read_word?
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
Lê um WORD mantendo as aspas.
A responsabilidade de interpretar/remover as aspas será do Expander.
*/
static t_token	*read_word(const char *line, size_t *i)
{
	size_t	start;
	size_t	len;
	int		in_single;
	int		in_double;
	char	*value;

	start = *i;
	in_single = 0;
	in_double = 0;
	//	Percorre a string para achar o fim da palavra
	while (line[*i])
	{
		// Se trocarmos de estado de aspas, apenas atualizamos a flag
		// Mas, continuamos avançando o índice (não damos continue)
		if (line[*i] == '\'' && !in_double)
			in_single = !in_single;
		else if (line[*i] == '\"' && !in_single)
			in_double = !in_double;
		// Se achou separador  E não está entre aspas, acabou a palavra
		else if (!in_single && !in_double &&
				(line[*i] == ' ' || line[*i] == '\t' ||
				line[*i] == '|' || line[*i] == '<' || line[*i] == '>'))
			break;
		(*i)++;
	}

	// Se sair do loop com aspas abertas -> Erro de sintaxe
	if (in_single || in_double)
	{
		printf("minishell: Syntax error: Unclosed quotes\n");
		return (NULL);
	}

	// Copia exatamente o que estava lá (incluindo as aspas)
	len = *i - start;
	value = ft_substr(line, start, len);
	return (token_new_owned(WORD, value));
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