/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:51:04 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/17 18:20:15 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Libraries */

# include <stdlib.h>			//	malloc, free,
# include <unistd.h>			//	write, access
# include <stdio.h>				//	printf, perror
# include <readline/readline.h>
# include <readline/history.h>

/* Structures */

typedef struct s_shell			// Estrutura principal do shell (vamos expandir depois)
{
	int	last_status;			// Armazena o ultimo exit-status (para o $? no futuro)
} t_shell;

/* Functions */

//	main.c
void	run_shell(t_shell *sh);	//	Função principal do loop do shell

#endif