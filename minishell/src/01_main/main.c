/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:51:11 by leoaguia          #+#    #+#             */
/*   Updated: 2026/01/08 00:52:41 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Decide se roda builtin no pai ou manda para o pipeline.
*/
void	execute_command(t_shell *sh, t_cmd *cmds)
{
	if (!cmds)
		return ;
	if (cmds->argv && cmds->argv[0] && !cmds->next)
	{
		if (ft_strcmp(cmds->argv[0], "cd") == 0)
			return ((void)(sh->last_status = ft_cd(sh, cmds)));
		if (ft_strcmp(cmds->argv[0], "export") == 0)
			return ((void)(sh->last_status = ft_export(sh, cmds)));
		if (ft_strcmp(cmds->argv[0], "unset") == 0)
			return ((void)(sh->last_status = ft_unset(sh, cmds)));
		if (ft_strcmp(cmds->argv[0], "exit") == 0)
			return ((void)(sh->last_status = ft_exit(sh, cmds)));
	}
	execute_pipeline(sh, cmds);
}

/*
Processa a linha: Lexer, Expander, Parser e Executor.
*/
static void	process_line(t_shell *sh, char *line)
{
	add_history(line);
	sh->tokens = tokenize_line(line);
	if (!sh->tokens)
		return ;
	expand_all_tokens(sh, sh->tokens);
	sh->cmds = parse_pipeline(sh->tokens);
	if (!sh->cmds)
	{
		free_tokens(sh->tokens);
		return ;
	}
	execute_command(sh, sh->cmds);
	free_cmds(sh->cmds);
	free_tokens(sh->tokens);
}

/*
Loop principal do shell com tratamento de sinal no prompt.
*/
void	run_shell(t_shell *sh)
{
	char	*line;

	setup_signals_interactive();
	while (1)
	{
		sh->tokens = NULL;
		sh->cmds = NULL;
		line = readline("minishell$ ");
		if (g_signal != 0)
		{
			sh->last_status = g_signal;
			g_signal = 0;
		}
		if (line == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (*line != '\0')
			process_line(sh, line);
		free(line);
	}
}

/*
Inicializacao e limpeza final.
*/
int	main(int argc, char **argv, char **envp)
{
	t_shell	sh;

	(void)argc;
	(void)argv;
	g_signal = 0;
	sh.last_status = 0;
	init_env(&sh, envp);
	increment_shell_level(&sh);
	run_shell(&sh);
	free_env_list(sh.env_list);
	rl_clear_history();
	return (sh.last_status);
}
