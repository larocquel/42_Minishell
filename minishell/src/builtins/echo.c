/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davmendo <davmendo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:52:38 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/05 21:43:09 by davmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(char **argv)
{
	int	i;
	int	nl;
	int	first_arg;
	int	j;

	i = 1;
	nl = 1;
	while (argv[i] && argv[i][0] == '-' && argv[i][1])
	{
		/* comeca em 1 para pular o sinal '-' */
		j = 1;
		/* consome todos os 'n' consecutivos */
		while (argv[i][j] == 'n')
			j++;
		/* se apos o 'n' sobrar algo nao e da flag... */
		if (argv[i][j] != '\0')
			break ;
		nl = 0;
		/* avanca para o proximo argumetno */
		i++;
	}
	/* guarda o indice do primeiro agrumento sem ser a flag */
	first_arg = i;
	while (argv[i])
	{
		/* se nao for o primeiro argumento a imprimir, escvere um espaco antes do primeiro argumetno */
		if (i > first_arg)
			write(1, " ", 1);
		write(1, argv[i], strlen(argv[i]));
		i++;
	}
	/*se nao foi encontrado nenhum flag -n imprime uma newline */
	if (nl)
		write(1, "\n", 1);
	return (0);
}
