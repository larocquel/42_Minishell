/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 14:17:00 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/04 19:50:04 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	OBS: pENSAR NA ALTERNATIVA DE MANTER AS ASPAR PARA DAR MAIS JEITO NAS EXPANSOES
//	"$USER" '$USER', QUESTAO -> ENVIAR COM OU SEM ASPAS?
//	ideia do Joao: meter um boolean como flag para sinalizar a expansao
//	indice do expansor
//	array de ints para o caso de haver mais de uma expansao


//	Whitespace
static int	tk_is_ws(int c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\r' || c == '\v' || c == '\f');
}
//	Operators
static int	tk_is_op(int c)
{
	return (c == '|' || c == '<' || c == '>');
}

static char	*tk_buf_add(char *buf, size_t *cap, size_t *len, char c, int *err)
{
	char	*new;
	size_t	i;

	//	1. Crescer se não houver espaço para +1 caractere
	if (*len + 1 >= *cap)
	{
		if (*cap == 0)
			*cap = 16;
		else
			*cap *= 2;
		new = malloc(*cap);
		if (!new)
		{
			*err = 1;
			free(buf);
			return (NULL);
		}
		//	Copia conteudo antigo
		i = 0;
		while (i < *len)
		{
			new[i] = buf[i];
			i++;
		}
		free(buf);
		buf = new;
	}
	//	2. Adiciona o char ao final
	buf[*len] = c;
	(*len)++;
	return (buf);
}

//	Cria um novo token
static t_token	*tk_new(t_toktype type, char *val)
{
	t_token *t;

	t = malloc(sizeof(*t));
	if (!t)
	{
		free(val);
		return (NULL);
	}
	t->type = type;
	t->val = val;
	t->next = NULL;
	return (t);
}

//	Lê um WORD removendo aspas. Para quando encontra ws, op ou fim.
//	Em aspas não fechadas: *err = 1 e retorna NULL.
static char	*tk_read_word(const char *s, size_t *i, int *err)
{
	char	*buf;	//	Strign em construção
	size_t	cap;	//	Capacidade do buffer
	size_t	len;	//	Tamanho usado no buffer
	char	quote;	//	Tipo de aspa atual: \' ou \"

	buf = NULL;
	cap = 0;
	len = 0;
	//	Percorre enquanto existir, não for ws ou op
	while (s[*i] && !tk_is_ws(s[*i]) && !tk_is_op(s[*i]))
	{
		//	Caso 1: Abriu aspas
		if (s[*i] == '\'' || s[*i] == '\"')
		{
			quote = s[*i];	//	Guarda qual aspa abriu
			(*i)++;			//	Pula a aspa de abertura
			while (s[*i] && s[*i] != quote)
			{
				buf = tk_buf_add(buf, &cap, &len, s[*i], err);
				if (*err)
					return (NULL);
				(*i)++;
			}
			//	Se não encontrou a aspa de fechamento: erro
			if (s[*i] != quote)
			{
				*err = 1;
				free(buf);
				return (NULL);
			}
			(*i)++;
		}
		//	Caso 2: Caractere normal fora de aspas
		else
		{
			buf = tk_buf_add(buf, &cap, &len, s[*i], err);
			if (*err)
				return (NULL);
			(*i)++;
		}
	}
	//	Fechar a string com '\0'
	buf = tk_buf_add(buf, &cap, &len, '\0', err);
	if (*err)
		return (NULL);
	return (buf);
}
//	Lê o "operador" começando em s[*i] e cria 1 token.
static t_token	*tk_read_op(const char *s, size_t *i)
{
	//	Caso 1: Pipe '|'
	if (s[*i] == '|')
	{
		(*i)++;	//	Consumimos 1 caractere
		return (tk_new(PIPE, NULL));
	}
	//	Caso 2: Redirect input '<' ou Heredoc "<<"
	if (s[*i] == '<')
	{
		if (s[*i + 1] == '<')	//	Encontrou um Heredoc "<<"
		{
			*i += 2;	//	Consumimos 2 caracteres
			return (tk_new(HEREDOC, NULL));
		}
		(*i)++;	//	So um '<' Redirect in
		return (tk_new(REDIR_IN, NULL));
	}
	//	Caso 3: Redirect output '>' ou Append ">>"
	if (s[*i] == '>')
	{
		if (s[*i + 1] == '>')	//	Encontrou um Append ">>"
		{
			*i += 2;
			return (tk_new(REDIR_APP, NULL));
		}
		(*i)++;	//	So um '>' Redirect out
		return (tk_new(REDIR_OUT, NULL));
	}
	//	se chegou aqui, não era operador esperado (em teoria não chega)
	return (NULL);
}

t_token	*lex_line(const char *s, int *err)
{
	t_token	*head;   /* primeiro token da lista */
	t_token	*tail;   /* último token (para encadear rápido) */
	t_token	*tmp;    /* token recém-criado nesta iteração */
	char	*val;    /* string retornada por tk_read_word (para WORD) */
	size_t	i;       /* índice de varredura da linha */

	/* 1) validação básica */
	if (!s || !err)
		return (NULL);

	head = NULL;
	tail = NULL;
	*err = 0;
	i = 0;

	/* 2) varre a linha */
	while (s[i])
	{
		/* pular espaços em branco */
		while (s[i] && tk_is_ws(s[i]))
			i++;
		/* se acabou a linha, termina */
		if (!s[i])
			break;

		/* 3) operador vs palavra */
		if (tk_is_op(s[i]))
		{
			/* operadores criam token diretamente */
			tmp = tk_read_op(s, &i);
			if (!tmp)
			{
				free_token_list(head);
				return (NULL);
			}
		}
		else
		{
			/* palavras: primeiro pega a string... */
			val = tk_read_word(s, &i, err);
			if (*err || !val)
			{
				free_token_list(head);
				return (NULL);
			}
			/* ...depois cria o token WORD com ela */
			tmp = tk_new(WORD, val);
			if (!tmp)
			{
				free(val);
				free_token_list(head);
				return (NULL);
			}
		}

		/* 4) encadear na lista */
		if (!head)
			head = tmp;
		else
			tail->next = tmp;
		tail = tmp;
	}

	/* 5) devolve a lista (ou NULL se não havia tokens) */
	return (head);
}


void	free_token_list(t_token *lst)
{
	t_token	*next;

	while(lst)
	{
		next = lst->next;
		free(lst->val);
		free(lst);
		lst = next;
	}
}