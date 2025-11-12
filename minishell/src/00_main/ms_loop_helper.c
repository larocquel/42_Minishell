/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_loop_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 16:17:52 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/09 16:53:25 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Conta quantos tokens iniciais são do tipo WORD (até encontrar um operador
** como pipe/redireção ou o fim da lista). Usado para saber o tamanho do vetor
** de argumentos que será alocado em args_from_tokens.
*/
static size_t	count_initial_words(t_token *lst)
{
	size_t	n;

	n = 0;
	while (lst && lst->type == WORD)
	{
		n++;
		lst = lst->next;
	}
	return (n);
}

/*
** Libera args parcialmente preenchido (0..i-1) e o próprio vetor.
** Retorna NULL para facilitar retorno direto em args_from_tokens.
*/
static char	**free_args(char **args, size_t i)
{
	size_t	j;

	j = 0;
	while (j < i)
	{
		free(args[j]);
		j++;
	}
	free(args);
	return (NULL);
}

/*
** Constrói um vetor de strings (terminado em NULL) apenas com os tokens WORD
** consecutivos do começo da lista. Este é um passo temporário até termos o
** parser completo (pipes/redireções). Retorna NULL em falha de alocação.
*/
char	**args_from_tokens(t_token *lst)
{
	size_t	n;
	size_t	i;
	char	**args;

	n = count_initial_words(lst);
	args = (char **)malloc(sizeof(char *) * (n + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (lst && lst->type == WORD)
	{
		if (lst->val)
			args[i] = strdup(lst->val);
		else
			args[i] = strdup("");
		if (!args[i])
			return (free_args(args, i));
		i++;
		lst = lst->next;
	}
	args[i] = NULL;
	return (args);
}

/*
** Executa um comando simples a partir de args (builtin ou programa externo).
** Não lida com pipes/redireções (será papel do parser e do executor).
*/
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
** Fluxo de uma linha:
**  - tokeniza com lex_line
**  - em erro de aspas: reporta e sai
**  - monta args apenas com WORDs iniciais
**  - executa comando simples
**  - libera recursos
*/
void	handle_line(t_shell *sh, char *line)
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
		return ;
	}
	args = args_from_tokens(toks);
	free_token_list(toks);
	if (!args || !args[0])
	{
		free_tokens(args);
		return ;
	}
	exec_simple(sh, args);
	free_tokens(args);
}
