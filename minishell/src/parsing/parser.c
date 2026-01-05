/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 16:59:10 by leoaguia          #+#    #+#             */
/*   Updated: 2026/01/01 18:50:09 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Adiciona redireção ao fim da lista
*/
static int	redir_add_back(t_redir **lst, t_redir *new_r)
{
	t_redir	*tmp;

	if (!new_r)
		return (0);
	if (!*lst)
	{
		*lst = new_r;
		return (1);
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_r;
	return (1);
}

/*
Cria um node de redir (duplica target)
*/
static t_redir	*redir_new(t_type type, const char *target)
{
	t_redir	*r;

	r = malloc(sizeof(t_redir));
	if (!r)
		return (NULL);
	r->type = type;
	r->target = ft_strdup(target);
	if (!r->target)
	{
		free(r);
		return (NULL);
	}
	r->next = NULL;
	return (r);
}

/*
- Conta quantos WORD aparecem até o próximo PIPE (ignorando redirs e seus alvos).
- Retorna o número de WORD; não altera a lista de tokens.
*/
static size_t	count_words_until_pipe(t_token *tok)
{
	size_t	count;

	count = 0;
	while (tok && tok->type != PIPE)
	{
		if (tok->type == WORD)
			count++;
		else if (tok->type == R_IN || tok->type == R_OUT
			|| tok->type == R_APP || tok->type == R_HDC)
		{
			// pula o token da redireção e também o próximo, que deve ser WORD.
			// - se não for WORD, será tratado no create)
			if (tok->next)
				tok = tok->next;
			else
				break;
		}
		tok = tok->next;
	}
	return (count);
}

/*
Cria um comando consumindo tokens a partir de *cur até PIPE (ou fim).
- Preenche argv com os WORDs encontrados
- Constrói lista de redirs a partir dos tokens R_*
- Atualiza *cur para o token STOP (PIPE ou NULL)

Em caso de erro de sintaxe (redir sem target) ou falha de alocação, retorna NULL.
OBS: caller deve liberar recursos (free_cmds) em caso de NULL.
*/
static t_cmd	*create_cmd_from_tokens(t_token **cur)
{
	t_cmd	*cmd;
	size_t	word_count;
	size_t	i;
	t_token	*tok;
	t_token	*start;
	t_redir	*r;

	if (!cur || !*cur)
		return (NULL);
	start = *cur;
	word_count = count_words_until_pipe(start);
	// Se não houver palavras nesse trecho -> erro de sintaxe (ex: " |" ou ">> |")
	// Porém permitiremos comandos que só contenham redireções (ex: > out) ?
	// O subject normalmente exige pelo menos um argv por comando; manteremos a regra: precisa de pelo menos 1 WORD.
	if (word_count == 0)
		return (NULL);
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->redirs = NULL;
	cmd->next = NULL;
	cmd->argv = malloc(sizeof(char *) * (word_count + 1));
	if (!cmd->argv)
	{
		free(cmd);
		return (NULL);
	}
	i = 0;
	tok = start;
	while (tok && tok->type != PIPE)
	{
		if (tok->type == WORD)
		{
			cmd->argv[i] = ft_strdup(tok->value);
			if (!cmd->argv[i])
			{
				// Cleanup parcial
				while (i > 0)
					free(cmd->argv[--i]);
				free(cmd->argv);
				free(cmd);
				return (NULL);
			}
			i++;
			tok = tok->next;
		}
		else if (tok->type == R_IN || tok->type == R_OUT
			|| tok->type == R_APP || tok->type == R_HDC)
		{
			// devemos ter um token alvo a seguir (WORD)
			if (!tok->next || tok->next->type != WORD)
			{
				//	Erro de sintaxe: Redirection sem target
				//	Libera argv parcial
				while (i > 0)
					free(cmd->argv[--i]);
				free(cmd->argv);
				free(cmd);
				return (NULL);
			}
			//	Cria redir e anexa
			{
				r = redir_new(tok->type, tok->next->value);
				if (!r)
				{
					while (i > 0)
						free(cmd->argv[--i]);
					free(cmd->argv);
					free(cmd);
					return (NULL);
				}
				if (!redir_add_back(&cmd->redirs, r))
				{
					free_redirs(r);
					while (i > 0)
						free(cmd->argv[--i]);
					free(cmd->argv);
					free(cmd);
					return (NULL);
				}
			}
			// Consumir token da redir e o token alvo (WORD)
			tok = tok->next->next;
		}
		else	// token inesperado (teoricamente não ocorrerá)
		{
			tok = tok->next;
		}
	}
	//	Finaliza argv
	cmd->argv[i] = NULL;
	//	Atualiza *cur para o ponto onde paramos(pode ser PIPE ou NULL)
	*cur = tok;
	return (cmd);
}

/*
Valida sintaxe simples do uso de PIPE e redireções
- Não pode começar por PIPE
- Não pode ter dois PIPEs seguidos
- Não pode terminar com PIPE
- Redir não pode ficar sem target (isso será detectado em create_cmd)
*/
static int	validate_syntax(t_token *tok)
{
	t_token	*prev;

	prev = NULL;
	if (tok && tok->type == PIPE)
		return (0);
	while (tok)
	{
		// Detectar PIPE duplicado
		if (tok->type == PIPE && (!prev || prev->type == PIPE))
			return (0);
		// Detectar redir no fim (sem alvo)
		if ((tok->type == R_IN || tok->type == R_OUT
				|| tok->type == R_APP || tok->type == R_HDC)
			&& (!tok->next || tok->next->type != WORD))
		{
			return (0);
		}
		prev = tok;
		tok = tok->next;
	}
	if (prev && prev->type == PIPE)
		return (0);
	return (1);
}

/*
Transforma lista de tokens em lista de comandos (t_cmd),
com argv e redirs preenchidos.
*/
t_cmd	*parse_pipeline(t_token *tokens)
{
	t_cmd	*first;
	t_cmd	*last;
	t_cmd	*new_cmd;
	t_token	*cur;

	if (!tokens)
		return (NULL);
	if (!validate_syntax(tokens))
		return (NULL);
	first = NULL;
	last = NULL;
	cur = tokens;
	while (cur)
	{
		new_cmd = create_cmd_from_tokens(&cur);
		if (!new_cmd)
		{
			free_cmds(first);
			return (NULL);
		}
		if (!first)
			first = new_cmd;
		else
			last->next = new_cmd;
		last = new_cmd;
		//	Se estamos num PIPE, pula ele e segue
		if (cur && cur->type == PIPE)
			cur = cur->next;
	}
	return (first);
}