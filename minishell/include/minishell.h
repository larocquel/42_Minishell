/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:51:04 by leoaguia          #+#    #+#             */
/*   Updated: 2025/12/27 21:21:39 by leoaguia         ###   ########.fr       */
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
# include <fcntl.h>					//	open, O_RDONLY, ...
# include "sys/wait.h"

/* Macros */
# define CTRL_C 130
# define NFOUND 127

/* Structures */

//	Var global para sinais, o extern avisa que essa var global será utilizada em algum .c, mas não cria outra cópia dela.
extern volatile sig_atomic_t	g_signal;

//	Espécie de MACROS para o lexer » WORD = 0, PIPE = 1, ...
typedef enum e_type
{
	WORD,
	PIPE,
	R_IN,   /* '<'   */
	R_OUT,  /* '>'   */
	R_APP,  /* '>>'  */
	R_HDC   /* '<<'  */
}	t_type;

//	Estrutura de tokens
typedef struct s_token
{
	t_type			type;
	char			*value;	// string com o conteúdo do token (ex: "ls", "arquivo.txt")
	struct s_token	*next;
}	t_token;

//	Estrutura de redireções
typedef struct s_redir
{
	t_type			type;    //	Usar R_IN / R_OUT / R_APP / R_HDC
	char			*target; //	Arquivo ou delimitador de heredoc
	struct s_redir	*next;
}	t_redir;

//	Estrutura de comandos
typedef struct s_cmd
{
	char			**argv;		//	Lista de argumentos (argv[0] = comando)
	t_redir			*redirs;	//	Lista de redireções
	struct s_cmd	*next;		//	Próximo comando no pipeline (NULL se for o último)
}	t_cmd;

//	Estrutra para variáveis de ambiente
//	Ex: "HOME=/home/leo" -> key="HOME", value="/home/leo"
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

// Estrutura principal do shell
typedef struct s_shell
{
	t_env	*env_list;		// Lista encadeada das vars de ambiente
	int		last_status;	// Armazena o ultimo exit-status (para o $? no futuro)
} t_shell;

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
void	free_redirs(t_redir *r);

//	env.c (Lógica de alto nível)
void	init_env(t_shell *sh, char **envp);
char	*get_env_value(t_env *env, char *key);
void	env_update(t_shell *sh, char *key, char *value);

//	env_utils.c (Manipulação da lista)
t_env	*env_new(char *key, char *value);
void	env_add_back(t_env **lst, t_env *new_node);
t_env	*env_get_node(t_env *env, char *key);
void	env_remove_node(t_shell *sh, char *key);

//	builtins.c
int     ft_env(t_shell *sh);
int     ft_pwd(void);
int		ft_exit(t_shell *sh, t_cmd *cmd);
int		ft_echo(t_cmd *cmd);

//	builtins_ops.c
int		ft_export(t_shell *sh, t_cmd *cmd);
int		ft_unset(t_shell *sh, t_cmd *cmd);
int		ft_cd(t_shell *sh, t_cmd *cmd);

//	exec_utils.c
char    *find_executable(char *cmd, t_env *env_list);
char    **env_to_array(t_env *env_list);
void    free_array(char **arr);

//	expand.c
void	expand_all_tokens(t_shell *sh, t_token *tokens);

// redirect.c
int		setup_redirects(t_cmd *cmd);

// heredoc.c
int		process_heredoc(char *delimiter);
char	*get_heredoc_file(void);

// executor.c
void	execute_pipeline(t_shell *sh, t_cmd *cmds);

#endif