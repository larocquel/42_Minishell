/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 19:26:06 by leoaguia          #+#    #+#             */
/*   Updated: 2025/12/08 20:07:39 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Função auxiliar para remover aspas de uma string FINAL.
Ex: 'ola' -> ola | "ola" -> ola | ola -> ola
*/
void	remove_quotes(char *str)
{
	char	*src;
	char	*dst;
	int		in_single;
	int		in_double;

	// Dica: Fazemos a alteração "in-place" (na própria string)
	// O resultado será sempre menor ou igual o original

	if (!str)
		return ;
	src = str;
	dst = str;
	in_single = 0;
	in_double = 0;
	while (*src)
	{
		if ((*src == '\'') && !in_double)
			in_single = !in_single;
		else if ((*src == '\"') && !in_single)
			in_double = !in_double;
		else
		{
			// Se não for uma aspa que está delimitando, copia pro destino
			*dst = *src;
			dst++;
		}
		src++;
	}
	*dst = '\0';
}
/*
Lógica simples de expansão (Por enquanto trata apenas $? e variáveis simples)
Nota: Esta função ainda não lida perfeitamente com "texto$VAR",
vamos aprimorar no próximo passo. Faremos o básico para testar.
*/
void	expand_token(t_token *tok, t_shell *sh)
{
	char *val;

	// Se começar com aspas simples, NÃO expande nada, só tira as aspas
	if (tok->value[0] == '\'')
	{
		remove_quotes(tok->value);
		return ;
	}

	// Se for $?, substitui pelo último status
	if (ft_strcmp(tok->value, "$?") == 0)
	{
		free(tok->value);
		tok->value = ft_itoa(sh->last_status);
		return ;
	}

	// Se começar com $, tenta buscar no env (Assumindo caso simple $VAR)
	if (tok->value[0] == '$')
	{
		// Pula o $ e busca
		val = get_env_value(sh->env_list, tok->value + 1);
		free(tok->value);
		if (val)
			tok->value = ft_strdup(val);
		else
			tok->value = ft_strdup("");	// Variável não existe vira vazio
		return ;
	}

	// Se não for nenhum caso especial, apenas remove as aspas (ex: "ls")
	remove_quotes(tok->value);
}

// Função principal chamada pelo Main
void	expand_all_tokens(t_shell *sh, t_token *tokens)
{
	t_token *tmp;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == WORD)
			expand_token(tmp, sh);
		tmp = tmp->next;
	}
}