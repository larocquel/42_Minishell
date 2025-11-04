/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 09:23:36 by leoaguia          #+#    #+#             */
/*   Updated: 2025/10/31 09:37:39 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEL " "

void	split_line(char *line)
{
	char	*tokens[6] = {0};
	int		position;

	position = 0;
	for (char	*token = strtok(line, DEL); token; token = strtok(NULL, DEL))
	{
		tokens[position++] = token;
	}

	position = 0;
	while (tokens[position])
		printf("%s\n", tokens[position++]);
}

int		main(void)
{
	char	s[] = "ls -la file.txt | cat";
	split_line(s);

	return (EXIT_SUCCESS);
}