/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 16:59:10 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/19 17:49:22 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Conta quantos tokens WORD existem até próximo PIPE ou fim.
Ex: "echo oi mundo | grep oi"
	-> Para o primeiro comando: ("echo", "oi", "mundo") = 3 WORDs
*/
static size_t	count_words_until_pipe(t_token *tok)
{
	size_t	count;

	count = 0;
	while (tok && tok->type != PIPE)
	{
		if (tok->type == WORD)
			count++;
		else
		{
			//	To do: Futura lógica de redirections aqui
		}
		tok = tok->next;
	}
	return (count);
}

/*
Cria um t_cmd para o trecho atual até o PIPE:
- Aloca argv com tamanho num_words + 1
- Duplica os valores dos WORDs
- argv termina em NULL
*/
static t_cmd	*create_cmd_from_tokens(t_token **cur)
{
	t_cmd	*cmd;
	size_t	word_count;
	size_t	i;
	t_token	*tok;

	word_count = count_words_until_pipe(*cur);
	if (word_count == 0)
		return (NULL);
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = malloc(sizeof(char *) * (word_count + 1));
	if (!cmd->argv)
	{
		free(cmd);
		return (NULL);
	}
	i = 0;
	tok = *cur;
	while (tok && tok->type != PIPE)
	{
		if (tok->type == WORD)
		{
			cmd->argv[i] = ft_strdup(tok->value);
			if (!cmd->argv[i])
			{
				while (i > 0)
					free(cmd->argv[--i]); // --i ou i--? pq --i nao liberta o primeiro i a entrar
				free(cmd->argv);
				free(cmd);
				return (NULL);
			}
			i++;
		}
		//	To do: No futuro, vamos tratar redireções antes do PIPE, aqui
		tok = tok->next;
	}
	cmd->argv[i] = NULL;
	cmd->next = NULL;
	*cur = tok;	//	Devolve o ponteiro parado no PIPE (ou NULL)
	return (cmd);
}

/*
Libera  toda lista de comandos.
*/
void	free_cmds(t_cmd *cmd)
{
	t_cmd	*next;
	size_t	i;

	while (cmd)
	{
		next = cmd->next;
		if (cmd->argv)
		{
			i = 0;
			while (cmd->argv[i])
			{
				free(cmd->argv[i]);
				i++;
			}
			free(cmd->argv);
		}
		free(cmd);
		cmd = next;
	}
}

/*
Valida sintaxe simples do uso de PIPE
- Não pode começar com PIPE
- Não pode ter dois PIPEs seguidos
- Não pode terminar com PIPE
*/
static int	validate_pipes_syntax(t_token *tok)
{
	t_token	*prev;

	prev = NULL;
	if (tok && tok->type == PIPE)
		return (0);
	while (tok)
	{
		if (tok->type == PIPE && (!prev || prev->type == PIPE))
			return (0);
		prev = tok;
		tok = tok->next;
	}
	if (prev && prev->type == PIPE)
		return (0);
	return (1);
}

/*
Constrói a lista de t?cmd a partir da lista de tokensÇ
- Tokens separados por PIPE viram comandos separados
- Por enquanto só tratamos WORD e PIPE
*/
t_cmd	*parse_pipeline(t_token *tokens)
{
	t_cmd	*first;
	t_cmd	*last;
	t_cmd	*new_cmd;
	t_token	*cur;

	if (!validate_pipes_syntax(tokens))
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