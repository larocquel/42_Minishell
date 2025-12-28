/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 12:26:05 by leoaguia          #+#    #+#             */
/*   Updated: 2025/12/28 00:54:20 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Gera um nome temporário para o arquivo do Heredoc.
Retorno: String fixa oculta
*/
static char	*get_tmp_filename(void)
{
	// Em shells reais, usaria um contador para suportar múltiplos heredocs
	// Para o básico, um nome fixo resolve.
	return (".heredoc_tmp");
}

/*
Loop principal do Heredoc:
1 - Abre o aruqivo temporário.
2 - Lê linha a linha com o readline.
3 - Se linha == delimitador, para.
4 - Se não, escreve a linha no arquivo.
*/
int	process_heredoc(char *delimiter)
{
	int		fd;
	char	*line;
	char	*filename;

	filename = get_tmp_filename();

	// Abre com permissão de escrita, cria se não existe, zera se existe
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	while (1)
	{
		// TODO: Se houve Ctrl-C globalmente, para tudo (implementaremos check melhor depois)
		if (g_signal == 130)
			break;
		line = readline("> ");
		if (!line)	// Ctrl-D (EOF) sem delimitador
		{
			printf("warning: here-document delimited by end-of-file\n");
			break ;
		}
		// Verifica de a linha é exatamente o delimitador
		// ft_strncmp compara tamanho + 1 para garantir match exato do \0
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter + 1)) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);	// Adiciona quebra de linha que o readline remove
	}
	close(fd);
	return (0);
}


/*
Função auxiliar: Recuperar o nome do arquivo tmp para ser usado no redirect.c
*/
char	*get_heredoc_file(void)
{
	return (get_tmp_filename());
}