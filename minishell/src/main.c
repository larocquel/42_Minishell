/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:51:11 by leoaguia          #+#    #+#             */
/*   Updated: 2025/12/27 22:49:18 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// // Debug: Adicione uma função temporária para testar se deu certo
// void debug_print_env(t_env *env)
// {
// 	while (env)
// 	{
// 		printf("KEY: %s | VAL: %s\n", env->key, env->value);
// 		env = env->next;
// 	}
// }

// // Debug: Imprime lista de redirs de um comando
// static void debug_print_redirs(t_redir *r)
// {
// 	const char *type_str;
// 	size_t i = 0;

// 	while (r)
// 	{
// 		if (r->type == R_IN)
// 			type_str = "R_IN";
// 		else if (r->type == R_OUT)
// 			type_str = "R_OUT";
// 		else if (r->type == R_APP)
// 			type_str = "R_APP";
// 		else if (r->type == R_HDC)
// 			type_str = "R_HDC";
// 		else
// 			type_str = "UNKNOWN";

// 		printf("  redir[%zu] = (%s) target=(%s)\n", i, type_str,
// 		       r->target ? r->target : "(null)");
// 		r = r->next;
// 		i++;
// 	}
// }

// // Debug: Imprime argv e redirs de cada comando
// static void	debug_print_cmds(t_cmd *cmds)
// {
// 	size_t i;

// 	while (cmds)
// 	{
// 		printf("CMD:\n");
// 		i = 0;
// 		if (cmds->argv)
// 		{
// 			while (cmds->argv[i])
// 			{
// 				printf("  argv[%zu] = (%s)\n", i, cmds->argv[i]);
// 				i++;
// 			}
// 		}
// 		else
// 			printf("  (argv == NULL)\n");

// 		// Imprime redireções se houver
// 		if (cmds->redirs)
// 			debug_print_redirs(cmds->redirs);
// 		else
// 			printf("  (no redirs)\n");

// 		cmds = cmds->next;
// 	}
// }

// // Debug: Retorna a string do tipo identificado
// static const char	*token_type_str(t_type type)
// {
// 	if (type == WORD)
// 		return ("WORD");
// 	if (type == PIPE)
// 		return ("PIPE");
// 	if (type == R_IN)
// 		return ("R_IN");
// 	if (type == R_OUT)
// 		return ("R_OUT");
// 	if (type == R_APP)
// 		return ("R_APP");
// 	if (type == R_HDC)
// 		return ("R_HDC");
// 	return ("UNKNOWN");
// }


// // Debug: imprime a lista de tokens no terminal.
// static void	debug_print_tokens(t_token *lst)
// {
// 	while (lst)
// 	{
// 		printf("[%-9s] (%s)\n", token_type_str(lst->type), lst->value);
// 		lst = lst->next;
// 	}
// }

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
Loop principal do shell
*/
void	run_shell(t_shell *sh)
{
	char	*line;
	t_token	*tokens;
	t_cmd	*cmds;

	setup_signals_interactive();
	while (1)
	{
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
		tokens = tokenize_line(line);
		if (!tokens)
		{
			free(line);
			continue;
		}

		// 2. EXPANDER (Novo!)
		// Transforma "$HOME" em "/home/leo" e limpa aspas
		expand_all_tokens(sh, tokens);

		// Debug (Opcional): Verifique como ficaram os tokens
		//debug_print_tokens(tokens);

        // 3. Parser
		cmds = parse_pipeline(tokens);
		if (!cmds)
		{
			free_tokens(tokens);
			free(line);
			continue;
		}

		// 4. EXECUÇÃO
		execute_command(sh, cmds);

		// Limpeza da rodada
		free_cmds(cmds);
		free_tokens(tokens);
		free(line);
	}
}

/*
main:
- Inicializa a struct do shell
- Chama run_shell
- Retorna o último status
*/
int	main(int argc, char **argv, char **envp) //	O que é envp?
{
	t_shell	sh;

	(void)argc;
	(void)argv;

	g_signal = 0;
	sh.last_status = 0;
	// 1. Inicializa o ambiente
	init_env(&sh, envp);
	run_shell(&sh);
	// TODO: Precisaremos de uma função free_env(&sh) no futuro para limpar a memória ao sair

	return (sh.last_status);
}