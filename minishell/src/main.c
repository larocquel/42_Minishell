/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:51:11 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/18 21:27:35 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Pra ver se os tokens estão saindo certo,
podemos temporariamente imprimir os tokens
*/
static void	debug_print_tokens(t_token *lst)
{
	while (lst)
	{
		printf("[type=%d]'%s'\n", lst->type, lst->value);
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
		free_tokens(tokens);

		//	Por enquanto só mostramos o que o user digitou
		printf("Digitou: %s\n", line);
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
	(void)envp;
	sh.last_status = 0;
	run_shell(&sh);
	return (sh.last_status);
}