/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:51:11 by leoaguia          #+#    #+#             */
/*   Updated: 2026/01/01 19:28:58 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Roteador de comandos:
Decide se o comando deve rodar no processo PAI (Builtins de estado)
ou se deve ir para o executor geral (Pipes e Externos).
*/
void	execute_command(t_shell *sh, t_cmd *cmds)
{
	// Se não houver comando, não faz nada
	if (!cmds || !cmds->argv || !cmds->argv[0])
		return ;

	// Caso especial: APENAS UM comando e é Builtin de estado.
	// Estes comandos alteram o shell atual (variáveis, diretório, saída),
	// por isso PRECISAM rodar no Pai, sem fork.
	// Se houver pipe (cmds->next), eles rodam no filho (via execute_pipeline)
	// para não matar o shell pai.
	if (!cmds->next)
	{
		if (ft_strcmp(cmds->argv[0], "cd") == 0)
		{
			sh->last_status = ft_cd(sh, cmds);
			return ;
		}
		if (ft_strcmp(cmds->argv[0], "export") == 0)
		{
			sh->last_status = ft_export(sh, cmds);
			return ;
		}
		if (ft_strcmp(cmds->argv[0], "unset") == 0)
		{
			sh->last_status = ft_unset(sh, cmds);
			return ;
		}
		if (ft_strcmp(cmds->argv[0], "exit") == 0)
		{
			// Passamos 'cmds' para verificar argumentos (ex: exit 42)
			sh->last_status = ft_exit(sh, cmds);
			return ;
		}
	}
	// Se não for um caso especial do pai, mandamos para o executor genérico.
	// O execute_pipeline cuida de criar forks, pipes e rodar builtins de display (echo, pwd...).
	execute_pipeline(sh, cmds);
}

/*
Loop principal do shell.
Lê a linha, processa tokens, expande variáveis, faz parsing e executa comandos.
*/
void	run_shell(t_shell *sh)
{
	char	*line;

	setup_signals_interactive();
	while (1)
	{
		sh->tokens = NULL;	// Inicializa ponteiros no início de cada ciclo
		sh->cmds = NULL;
		line = readline("minishell$ ");
		if (line == NULL) // CTRL-D
		{
			printf("exit\n");
			break ;
		}
		if (*line == '\0')
		{
			free(line);
			continue;
		}
		add_history(line);

        // 1. Lexer
		sh->tokens = tokenize_line(line);	// Armazena tokens no struct
		if (!sh->tokens)
		{
			free(line);
			continue;
		}

		// 2. Expander
		expand_all_tokens(sh, sh->tokens);

        // 3. Parser
		sh->cmds = parse_pipeline(sh->tokens);	// Armazena comandos no struct
		if (!sh->cmds)
		{
			free_tokens(sh->tokens);
			free(line);
			continue;
		}

		// 4. EXECUÇÃO
		execute_command(sh, sh->cmds);

		// Limpeza da rodada
		free_cmds(sh->cmds);
		free_tokens(sh->tokens);
		free(line);
	}
}

/*
Ponto de entrada do programa.
- Inicializa o shell.
- Carrega ambiente.
- Ajusta SHLVL.
- Entra no loop principal.
- Limpeza final.
*/
int	main(int argc, char **argv, char **envp) //	O que é envp?
{
	t_shell	sh;

	(void)argc;
	(void)argv;

	g_signal = 0;
	sh.last_status = 0;
	// 1. Carrega o ambiente
	init_env(&sh, envp);

	// 2. Incrementa o SHLVL
	increment_shell_level(&sh);

	// 3. Entra no loop
	run_shell(&sh);

	// Limpeza final do ambiente (caso saia do loop run_shell normalmente)
	free_env_list(sh.env_list);
	rl_clear_history();

	return (sh.last_status);
}
