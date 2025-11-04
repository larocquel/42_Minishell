/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:51:46 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/04 19:32:22 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ms_update_interactive(t_shell *sh)
{
	sh->interactive = isatty(STDIN_FILENO);
}

/* TEMP: cria argv apenas com WORDs até encontrar operador (PIPE/REDIR/...) */
static char	**argv_from_tokens(t_token *lst)
{
	size_t	n;
	size_t	i;
	t_token	*cur;
	char	**argv;

	n = 0;
	cur = lst;
	while (cur && cur->type == WORD)
	{
		n++;
		cur = cur->next;
	}
	argv = (char **)malloc(sizeof(char *) * (n + 1));
	if (!argv)
		return (NULL);
	i = 0;
	cur = lst;
	while (cur && cur->type == WORD)
	{
		argv[i++] = strdup(cur->val ? cur->val : "");
		cur = cur->next;
	}
	argv[i] = NULL;
	return (argv);
}


void	repl_loop(t_shell *sh)
{
	char	*line;
	t_token	*toks;
	int		err;
	char	**argv;

	ms_update_interactive(sh);
	signals_setup_interactive();
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			if (sh->interactive)
				printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);

		/* 1) Tokenizar com o novo lexer */
		toks = lex_line(line, &err);
		free(line);
		if (err)
		{
			fprintf(stderr, "minishell: syntax error: unclosed quotes\n");
			free_token_list(toks);
			continue ;
		}
		if (!toks)
		{
			continue ;
		}

		/* 2) TEMP: montar argv com WORDs (até termos parser de verdade) */
		argv = argv_from_tokens(toks);
		if (!argv || !argv[0])
		{
			free_tokens(argv);
			free_token_list(toks);
			continue ;
		}

		/* 3) Execução igual ao que você já tinha */
		if (is_builtin(argv[0]))
			sh->last_status = exec_builtin(sh, argv);
		else
			sh->last_status = exec_external(sh, argv);

		/* 4) Limpezas */
		free_tokens(argv);
		free_token_list(toks);
	}
}

