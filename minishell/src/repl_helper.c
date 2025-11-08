/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 16:17:52 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/08 16:42:21 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/* Conta WORDs consecutivos no começo da lista de tokens. */

static size_t	count_leading_words(t_token *lst)
{
	size_t	n;
	t_token	*cur;

	n = 0;
	cur = lst;
	while (cur && cur->type == WORD)
	{
		n++;
		cur = cur->next;
	}
	return (n);
}


/* 	Libera parcialmente args[0..i-1] e o vetor args. */

static void	free_partial_args(char **args, size_t i)
{
	size_t	j;

	j = 0;
	while (j < i)
	{
		free(args[j]);
		j++;
	}
	free(args);
}
/*
Converte WORDs iniciais em vetor de argumentos terminado em NULL.
Retorna NULL em falha de alocação.
 */
char	**args_from_tokens(t_token *lst)
{
	size_t	n;
	size_t	i;
	t_token	*cur;
	char	**args;

	n = count_leading_words(lst);
	args = (char **)malloc(sizeof(char *) * (n + 1));
	if (!args)
		return (NULL);
	i = 0;
	cur = lst;
	while (cur && cur->type == WORD)
	{
		if (cur->val)
			args[i] = strdup(cur->val);
		else
			args[i] = strdup("");
		if (!args[i])
		{
			free_partial_args(args, i);
			return (NULL);
		}
		i++;
		cur = cur->next;
	}
	args[i] = NULL;
	return (args);
}


/* Executa comando simples (builtin ou externo) a partir de args. */

void	exec_simple(t_shell *sh, char **args)
{
	if (!args)
		return ;
	if (!args[0])
		return ;
	if (is_builtin(args[0]))
		sh->last_status = exec_builtin(sh, args);
	else
		sh->last_status = exec_external(sh, args);
}

/*
Processa uma linha:
- Tokeniza; trata erro de aspas.
- Constrói args a partir de WORDs.
- Executa comando simples.
Retorna 0 sempre (valor de retorno reservado se quiser evoluir).
 */
int	handle_line(t_shell *sh, char *line)
{
	t_token	*toks;
	char	**args;
	int		err;

	err = 0;
	toks = lex_line(line, &err);
	free(line);
	if (err || !toks)
	{
		free_token_list(toks);
		if (err)
			fprintf(stderr, "minishell: syntax error: unclosed quotes\n");
		return (0);
	}
	args = args_from_tokens(toks);
	free_token_list(toks);
	if (!args || !args[0])
	{
		free_tokens(args);
		return (0);
	}
	exec_simple(sh, args);
	free_tokens(args);
	return (0);
}
