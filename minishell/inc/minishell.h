/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:51:19 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/12 23:21:22 by leoaguia         ###   ########.fr       */
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

/*	MACROS	*/

# define NQ 0
# define SQ 1
# define DQ 2


/*	TYPES	*/

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
	unsigned char	*qmask;
	struct s_token	*next;
}	t_token;

typedef struct s_str
{
	char	*data;
	size_t	cap;
	size_t	len;
}	t_str;

typedef struct s_mask
{
	unsigned char	*data;
	size_t			cap;
	size_t			len;
}	t_mask;

typedef struct s_reader
{
	const char	*s;
	size_t		*i;
	int			*err;
}	t_reader;

typedef struct s_acc
{
	t_str		*b;
	t_mask		*m;
}	t_acc;


/* ****************************    src/00_main/    ************************** */

//	main.c
int				main(int ac, char **av, char **envp);

//	ms_init.c
void			ms_init(t_shell *sh, char **envp);
void			ms_destroy(t_shell *sh);

//	ms_loop_helper.c
char			**args_from_tokens(t_token *lst);
void			exec_simple(t_shell *sh, char **args);
void			handle_line(t_shell *sh, char *line);

//	ms_loop.c
void			ms_loop(t_shell *sh);

/* ****************************    src/01_parse/    ************************* */

//	token.c
t_token			*lex_line(const char *s, int *err);
void			free_token_list(t_token *lst);
void			free_tokens(char **toks);


//	token_utils.c
int				tk_is_space(int c);
int				tk_is_operator(int c);
t_token			*tk_new(t_toktype type, char *val, unsigned char *mask);
void			tk_push(t_token **head, t_token **tail, t_token *node);

//	token_read.c
t_token			*tk_read_op(const char *s, size_t *i);
char			*tk_read_word(const char *s, size_t *i, int *err, unsigned char **out_mask);

//	token_buf.c
char			*tk_add_str(t_str *b, char c, int *err);
unsigned char	*tk_add_mask(t_mask *m, unsigned char v, int *err);

/* ****************************    src/02_exec/    ************************** */

//	exec.c
int				is_builtin(const char *cmd);
int				exec_external(t_shell *sh, char **argv);

//	path.c
char			*ms_search_path(char **envp, const char *cmd);

/* ***************************    src/03_builtins/    *********************** */

//	builtins.c
int				exec_builtin(t_shell *sh, char **argv);

//	cd.c
int				builtin_cd(t_shell *sh, char **argv);

//	echo.c
int				builtin_echo(char **argv);


//	env.c
int				builtin_env(t_shell *sh);


//	exit.c
int				builtin_exit(t_shell *sh, char **argv);


//	export.c
int				builtin_export(t_shell *sh, char **argv);

//	pwd.c
int				builtin_pwd(void);

//	unset.c
int				builtin_unset(t_shell *sh, char **argv);

/* ***************************    src/04_signals/    ************************ */

//	signals.c
void			sigint_handler(int signo);
void			signals_setup_interactive(void);
void			signals_setup_child(void);

/* ****************************    src/05_utils/    ************************* */

//	utils_env.c
char			**ms_env_dup(char **envp);
void			ms_free_strarr(char **arr);
char			*ms_getenv(char **envp, const char *key);
int				ms_setenv(t_shell *sh, const char *k, const char *v);
int				ms_unsetenv(t_shell *sh, const char *key);

//	utils_num.c
int				ms_isnumber(const char *s);

//	utils_str.c
size_t			ms_split_count(char **arr);
char			*ms_strjoin3(const char *a, const char *b, const char *c);

#endif
