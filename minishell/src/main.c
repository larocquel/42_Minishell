/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:51:11 by leoaguia          #+#    #+#             */
/*   Updated: 2025/12/08 19:57:46 by leoaguia         ###   ########.fr       */
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

// Debug: Retorna a string do tipo identificado
static const char	*token_type_str(t_type type)
{
	if (type == WORD)
		return ("WORD");
	if (type == PIPE)
		return ("PIPE");
	if (type == R_IN)
		return ("R_IN");
	if (type == R_OUT)
		return ("R_OUT");
	if (type == R_APP)
		return ("R_APP");
	if (type == R_HDC)
		return ("R_HDC");
	return ("UNKNOWN");
}


// Debug: imprime a lista de tokens no terminal.
static void	debug_print_tokens(t_token *lst)
{
	while (lst)
	{
		printf("[%-9s] (%s)\n", token_type_str(lst->type), lst->value);
		lst = lst->next;
	}
}

// Função para comando externos
void	execute_external(t_shell *sh, t_cmd *cmd)
{
	pid_t	pid;
	char	*path;
	char	**env_array;
	int		status;

	// 1. Achar o caminho
	path = find_executable(cmd->argv[0], sh->env_list);
	if (!path)
	{
		printf("minishell: %s: command not found\n", cmd->argv[0]);
		sh->last_status = NFOUND; // NFOUND = 127 é o código padrão para "command not found"
		return ;
	}

	// 2. Fork
	pid = fork();
	// Processo FILHO
	if (pid == 0)
	{
		// Converter env list para array
		env_array = env_to_array(sh->env_list);

		// Executar (Se der certo, o programa "substitui" o filho e não retorna)
		execve(path, cmd->argv, env_array);

		// Se chegou aqui, execve falhou (ex: permissão negada, formato errado)
		perror("minishell: execve failed");
		free(path);
		free_array(env_array);
		// Importante: Filho deve sair, senão ele continua rodando o shell!
		exit(1);
	}
	// Processo PAI
	else if (pid > 0)
	{
		// Espera o filho terminar
		waitpid(pid, &status, 0);

		// Extrai o exit status (macro WEXITSTATUS)
		if (WIFEXITED(status))
			sh->last_status = WEXITSTATUS(status);

		free(path);
	}
	else
		perror("fork failed");

}

// Roteador do comandos
void	execute_command(t_shell *sh, t_cmd *cmd)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return ;

	// Comparação simples para rotear os builtins
	if (ft_strcmp(cmd->argv[0], "env") == 0)
		sh->last_status = ft_env(sh);
	else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		sh->last_status = ft_pwd();
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		sh->last_status = ft_exit(sh);
	else
		execute_external(sh, cmd);
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
		debug_print_tokens(tokens);

        // 3. Parser
		cmds = parse_pipeline(tokens);
		if (!cmds)
		{
			free_tokens(tokens);
			free(line);
			continue;
		}

		// 3. EXECUÇÃO (Nosso Executor Temporário)
        // Por enquanto, só executa o primeiro comando, ignorando pipes
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
- Retorna o último status (por enquanto sempre 0)
*/
int	main(int argc, char **argv, char **envp) //	O que é envp?
{
	t_shell	sh;

	(void)argc;
	(void)argv;

	sh.last_status = 0;
	// 1. Inicializa o ambiente
	init_env(&sh, envp);
	run_shell(&sh);
	// TODO: Precisaremos de uma função free_env(&sh) no futuro para limpar a memória ao sair

	return (sh.last_status);
}