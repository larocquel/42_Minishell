#include "cell.h"

int	cell_exit(char **args)
{
	(void)	args;
	loading();
	exit(EXIT_SUCCESS);
}

// int	cell_env(char **args)
// {
// 	(void)		args;
// 	extern char	**enviro;

// 	if (!enviro)
// 		return (1);
// 	for (int i = 0; enviro[i]; i++)
// 		printf("%s\n", enviro[i]);
// 	return (0);
// }