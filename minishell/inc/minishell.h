/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davmendo <davmendo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:51:19 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/06 16:34:34 by davmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

extern volatile sig_atomic_t	g_sig;

typedef struct s_shell
{
	char	**envp;
	int		last_status;
	int		interactive;
}	t_shell;

typedef enum e_toktype
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APP,
	HEREDOC
}	t_toktype;

typedef struct s_token
{
	t_toktype		type;
	char			*val;
	struct s_token	*next;
}	t_token;

/* core */
void	shell_init(t_shell *sh, char **envp);
void	shell_destroy(t_shell *sh);
void	repl_loop(t_shell *sh);

/* signals */
void	signals_setup_interactive(void);
void	signals_setup_child(void);
void	sigint_handler(int signo);

/* simple parser + future lexer */
char	**tokenize_simple(const char *line);
void	free_tokens(char **toks);
t_token	*lex_line(const char *s, int *err);
void	free_token_list(t_token *lst);
char	**tokens_to_argv(t_token *lst);

/* tokenizer */
t_token	*lex_line(const char *s, int *err);
void	free_token_list(t_token *lst);

/* exec */
int		is_builtin(const char *cmd);
int		exec_builtin(t_shell *sh, char **argv);
int		exec_external(t_shell *sh, char **argv);
char	*ms_search_path(char **envp, const char *cmd);

/* utils */
size_t	ms_split_count(char **arr);
char	*ms_strjoin3(const char *a, const char *b, const char *c);
char	**ms_env_dup(char **envp);
void	ms_free_strarr(char **arr);
char	*ms_getenv(char **envp, const char *key);
int		ms_setenv(t_shell *sh, const char *k, const char *v);
int		ms_unsetenv(t_shell *sh, const char *key);
int		ms_isnumber(const char *s);

/* builtins */
int		builtin_echo(char **argv);
int		builtin_pwd(char **argv);
int		builtin_env(t_shell *sh, char **argv);
int		builtin_exit(t_shell *sh, char **argv);
int		builtin_cd(t_shell *sh, char **argv);
int		builtin_export(t_shell *sh, char **argv);
int		builtin_unset(t_shell *sh, char **argv);

#endif
