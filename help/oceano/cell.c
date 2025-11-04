/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cell.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 17:12:11 by leoaguia          #+#    #+#             */
/*   Updated: 2025/10/31 17:59:37 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cell.h"

//	Global

int			status = 0;

t_builtin	g_builtin[] =
{
	// {.builtin_name="echo", .foo=cell_echo},
	// {.builtin_name="env", .foo=cell_env},
	{.builtin_name="exit", .foo=cell_exit},
	{.builtin_name=NULL},
};

void	cell_launch(char **args)
{
	if (Fork() == CELL_JR)
		Execvp(args[0], args);
	else
		Wait(&status);
}

void	cell_exec(char **args)
{
	const char	*curr;
	int			i;

	i = 0;
	while ((curr = g_builtin[i].builtin_name))
	{
		if (!strcmp(curr, args[0]))
		{
			status = g_builtin[i].foo(args);	//	TODO
			return ;
		}
		++i;
	}
	cell_launch(args);	//	TODO
}

char	**cell_split_line(char *line)
{
	char			**tokens;
	unsigned int	position;
	unsigned int	bufsize;

	bufsize = BUFSIZ;
	tokens = Malloc(bufsize * sizeof * tokens);
	position = 0;
	for (char *token = strtok(line, DEL); token; token = strtok(NULL, DEL))
	{
		tokens[position++] = token;
		if (position > bufsize)
		{
			bufsize *= 2;
			tokens = Realloc(tokens, bufsize);
		}
	}
	tokens[position] = NULL;

	return (tokens);
}

char	*cell_read_line(void)
{
	char	*buf;
	size_t	bufsize;
	char	cwd[BUFSIZ];

	buf = NULL;
	Getcwd(cwd, sizeof(cwd));
	printf(BLU"%s "GRE"$> "RST, cwd);

	if (getline(&buf, &bufsize, stdin) == -1)
	{
		free(buf);
		buf = NULL;
		if (feof(stdin))
			printf(RED"[EOF]"RST);
		else
			printf(RED"getline failed"RST);
	}
	return (buf);
}

int	main()
{
	//	REPL = Read -> Evaluate -> Print/Execute -> Loop
	char	*line;
	char	**args;

	printbanner();
	//	1 - Prompt + Get line -> DONE
	while ((line = cell_read_line()))
	{
		//	2 - Get tokens gettok (in future will use ->lexing->parsing EVALUATING)
		args = cell_split_line(line);
		/*	TEST	*/
		// for (int i = 0; args[i]; i++)
		// 	printf("%s\n", args[i]);

		//	3 - Execute
		cell_exec(args);

		//	4 - free
		free(line);
		free(args);
	}

	return (EXIT_SUCCESS);
}