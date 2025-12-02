/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:51:11 by leoaguia          #+#    #+#             */
/*   Updated: 2025/12/01 14:58:20 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Adicione uma função temporária para testar se deu certo (Debug)
void debug_print_env(t_env *env)
{
	while (env)
	{
		printf("KEY: %s | VAL: %s\n", env->key, env->value);
		env = env->next;
	}
}

// Imprime lista de redirs de um comando
static void debug_print_redirs(t_redir *r)
{
	const char *type_str;
	size_t i = 0;

	while (r)
	{
		if (r->type == R_IN)
			type_str = "R_IN";
		else if (r->type == R_OUT)
			type_str = "R_OUT";
		else if (r->type == R_APP)
			type_str = "R_APP";
		else if (r->type == R_HDC)
			type_str = "R_HDC";
		else
			type_str = "UNKNOWN";

		printf("  redir[%zu] = (%s) target=(%s)\n", i, type_str,
		       r->target ? r->target : "(null)");
		r = r->next;
		i++;
	}
}

// Imprime argv e redirs de cada comando
static void	debug_print_cmds(t_cmd *cmds)
{
	size_t i;

	while (cmds)
	{
		printf("CMD:\n");
		i = 0;
		if (cmds->argv)
		{
			while (cmds->argv[i])
			{
				printf("  argv[%zu] = (%s)\n", i, cmds->argv[i]);
				i++;
			}
		}
		else
			printf("  (argv == NULL)\n");

		// Imprime redireções se houver
		if (cmds->redirs)
			debug_print_redirs(cmds->redirs);
		else
			printf("  (no redirs)\n");

		cmds = cmds->next;
	}
}



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
/*
Função auxiliar para checar se a string é "exit"-
Mais pra frente isso vira um builtin de verdade.
*/
static int	is_exit_cmd(char *line)
{
	if (!line)
		return (0);
	//	Comparação simples
	if (line[0] == 'e' && line[1] == 'x' && line[2] == 'i' && line[3] == 't' && line[4] == '\0')
		return (1);
	return (0);
}

/*
Loop principal do shell
- Mostra o prompt
- Lê uma linha com readline
- Se EOF (ctrl-D), sai
- Se "exit", sai
- Senão, por enquanto, só imprime o que for digitado
*/
void	run_shell(t_shell *sh)
{
	char	*line;
	t_token	*tokens;
	t_cmd	*cmds;

	(void)sh;	//	Ainda não usamos
	setup_signals_interactive();	// Ativa os handlers (modo interativo)
	while (1)
	{
		//	readline() - Mostra o prompt e espera o user digitar
		line = readline("minishell$ ");

		//	Se line == NULL, significa EOF (ctrl-D)
		if (line == NULL)
		{
			printf("exit\n");
			break ;
		}

		// CTRL-C ou se só deu ENTER, a linha pode vir vazia -> só ignora
		if (*line == '\0')
		{
			free(line);
			continue;
		}

		//	Se a linha não for vazia, adiciona ao histórico
		add_history(line);

		//	Se o comando for "exit", sai do loop
		if (is_exit_cmd(line))
		{
			free(line);
			printf("exit\n");
			break ;
		}

		//	Lexer: tranforma a linha em tokens
		tokens = tokenize_line(line);
		if (!tokens)
		{
			printf("Erro no lexer\n");
			free(line);
			continue;
		}

		//	Debug: imprime tokens
		debug_print_tokens(tokens);

		//	Parser: Verifica sintaxe e separa linha em cmds
		cmds = parse_pipeline(tokens);
		if (!cmds)
		{
			printf("Erro no parser\n");
			free_tokens(tokens);
			free(line);
			continue;
		}
		//	Debug: imprime cmds
		debug_print_cmds(cmds);

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

	// DEBUG: Descomente para ver se funcionou, depois apague
	//debug_print_env(sh.env_list);

	run_shell(&sh);

	// TODO: Precisaremos de uma função free_env(&sh) no futuro para limpar a memória ao sair

	return (sh.last_status);
}