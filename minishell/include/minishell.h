/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:51:04 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/19 18:04:37 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Libraries */

# include "libft.h"
# include <stdlib.h>				//	malloc, free,
# include <unistd.h>				//	write, access
# include <stdio.h>					//	printf, perror
# include <signal.h>				//	sig_atomic_t, signals
# include <readline/readline.h>
# include <readline/history.h>

/* Macros */
# define CTRL_C 130

/* Structures */

//	Var global para sinais, o extern avisa que essa var global será utilizada em algum .c, mas não cria outra cópia dela.
extern volatile sig_atomic_t	g_signal;

// Estrutura principal do shell (vamos expandir depois)
typedef struct s_shell
{
	int	last_status;				// Armazena o ultimo exit-status (para o $? no futuro)
} t_shell;

//	Espécie de MACROS para o lexer » WORD = 0, PIPE = 1, ...
typedef enum e_tktype
{
	WORD,
	PIPE,
	R_IN,
	R_OUT,
	R_APP,
	R_HDC
}	t_tktype;

//	Estrutura de tokens
typedef struct s_token
{
	t_tktype		type;
	char			*value;	// string com o conteúdo do token (ex: "ls", "arquivo.txt")
	struct s_token	*next;
} t_token;

//	Estrutura de comandos
typedef struct s_cmd
{
	char			**argv;	//	Lista de argumentos (argv[0] = comando)
	struct s_cmd	*next;	//	Próximo comando no pipeline (NULL se for o último)
} t_cmd;



/* Functions */

//	main.c
void	run_shell(t_shell *sh);		//	Função principal do loop do shell

//	signals.c
void	setup_signals_interactive(void);

//	lexer.c
//	(primeira versão, sem aspas)
t_token	*tokenize_line(const char *line);
void	free_tokens(t_token *lst);

//	parser.c
t_cmd	*parse_pipeline(t_token *tokens);
void	free_cmds(t_cmd *cmd);

#endif