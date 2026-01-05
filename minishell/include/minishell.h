/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:51:04 by leoaguia          #+#    #+#             */
/*   Updated: 2026/01/05 16:44:46 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Libraries */
# include "libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/wait.h>

/* Structures */

/* Var global para sinais */
extern volatile sig_atomic_t	g_signal;

/* Macros para o lexer (WORD = 0, PIPE = 1, ...) */
typedef enum e_type
{
	WORD,
	PIPE,
	R_IN,
	R_OUT,
	R_APP,
	R_HDC
}	t_type;

/* Estrutura de tokens */
typedef struct s_token
{
	t_type			type;
	char			*value;
	struct s_token	*next;
}	t_token;

/* Estrutura de redirecoes */
typedef struct s_redir
{
	t_type			type;
	char			*target;
	struct s_redir	*next;
}	t_redir;

/* Estrutura de comandos */
typedef struct s_cmd
{
	char			**argv;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

/* Estrutura para variaveis de ambiente */
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

/* Estrutura principal do shell */
typedef struct s_shell
{
	t_env			*env_list;
	int				last_status;
	struct s_token	*tokens;
	struct s_cmd	*cmds;
}	t_shell;

/* Functions */

/* main.c */
void	run_shell(t_shell *sh);

/* signals.c */
void	setup_signals_interactive(void);
void	setup_signals_child(void);
void	setup_signals_ignore(void);

/* lexer.c */
t_token	*tokenize_line(const char *line);

/* parser.c */
t_cmd	*parse_pipeline(t_token *tokens);

/* env.c */
void	init_env(t_shell *sh, char **envp);
char	*get_env_value(t_env *env, char *key);
void	env_update(t_shell *sh, char *key, char *value);
void	increment_shell_level(t_shell *sh);

/* env_utils.c */
t_env	*env_new(char *key, char *value);
void	env_add_back(t_env **lst, t_env *new_node);
t_env	*env_get_node(t_env *env, char *key);
void	env_remove_node(t_shell *sh, char *key);

/* builtins.c */
int		ft_echo(t_cmd *cmd);
int		ft_pwd(void);
int		ft_env(t_shell *sh);
int		ft_exit(t_shell *sh, t_cmd *cmd);

/* builtins_ops.c */
int		ft_cd(t_shell *sh, t_cmd *cmd);
int		ft_export(t_shell *sh, t_cmd *cmd);
int		ft_unset(t_shell *sh, t_cmd *cmd);

/* builtins_utils.c */
int		is_numeric(char *str);
int		check_n_flag(char *arg);
void	update_wd_env(t_shell *sh, char *old);
void	print_export_error(char *arg);
int		is_valid_key(char *str);

/* exec_utils.c */
char	*find_executable(char *cmd, t_env *env_list);
char	**env_to_array(t_env *env_list);

/* expand.c */
void	expand_all_tokens(t_shell *sh, t_token *tokens);

/* redirect.c */
int		setup_redirects(t_cmd *cmd);

/* heredoc.c */
int		process_heredoc(char *delimiter);
char	*get_heredoc_file(void);

/* executor.c */
void	execute_pipeline(t_shell *sh, t_cmd *cmds);

/* cleanup.c */
void	free_tokens(t_token *lst);
void	free_array(char **arr);
void	free_cmds(t_cmd *cmd);
void	free_redirs(t_redir *r);
void	free_env_list(t_env *env);

#endif