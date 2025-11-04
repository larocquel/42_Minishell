#ifndef CELL_H
# define CELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <string.h>
# include <sysexits.h>

/*		STRUCTURES		*/

//	BUILT_INs
typedef struct	s_builtin
{
	const char	*builtin_name;
	int			(*foo)(char **);

}	t_builtin;

/*		MACROS DEFINITIONS		*/

//	COLORs

# define RST	"\033[0m"
# define RED	"\033[1;31m"
# define GRE	"\033[1;32m"
# define BLU	"\033[0;34m"
# define YEL	"\033[1;33m"
# define CIA	"\033[1;36m"

//	DELIMITATORs

# define DEL	" \t\n\v\f\r"

//	CHILD_PROCESS
# define CELL_JR 0

/*		FUNCTIONS AND PROTOTYPES		*/

//	Utils
void	printbanner(void);
void	loading(void);
void	Getcwd(char *buf, size_t size);
void	*Malloc(size_t size);
void	*Realloc(void *ptr, size_t size);
pid_t	Fork(void);
void	Execvp(const char *file, char *const argv[]);
pid_t	Wait(int *status);


//	Built-ins
int		cell_exit(char **args);
int		cell_env(char **args);


#endif