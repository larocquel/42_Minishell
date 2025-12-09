/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 19:26:06 by leoaguia          #+#    #+#             */
/*   Updated: 2025/12/09 16:56:54 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Adiciona um caractere ao final de uma string.
Libera a string antiga e retorna a nova.
*/
static char	*append_char(char *str, char c)
{
	char	*new_str;
	size_t	len;

	if (!str)
		str = ft_strdup("");
	len = ft_strlen(str);
	new_str = malloc(len + 2);	// +1 char +1 null
	if (!new_str)
	{
		free(str);
		return (NULL);
	}
	ft_strlcpy(new_str, str, len + 1);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(str);
	return (new_str);
}

/*
Adiciona uma string (val) ao final de outra (base).
Libera a base antiga.
*/
static char	*append_str(char *base, char *val)
{
	char	*new_str;

	if (!base)
		base = ft_strdup("");
	if (!val)
		return (base);	// Nada a adicionar
	// Posso usar ft_strjoin ou ft_strjoin3
	// strjoin normal vaza se não fizermos o free(base) manual
	new_str = ft_strjoin(base, val);
	free(base);
	return (new_str);
}

/*
- Descobre o nome da variável após o $
- Busca no env
- Retorna o novo índice i
Ex: "ola $USER fim" -> acha USER, retorna valor "leoaguia", atualiza i.
*/
static int	handle_dollar(char *str, int i, char **res, t_shell *sh)
{
	int		start;
	char	*var_name;
	char	*var_value;

	i++;	// Pula o $
	if (str[i] == '?')	// Caso especial $?
	{
		var_value = ft_itoa(sh->last_status);
		*res = append_str(*res, var_value);
		free(var_value);
		return (i + 1);
	}

	// Se for apenas $ sozinho ou seguido de espaço, não expande
	if (!ft_isalnum(str[i]) && str[i] != '_')
	{
		*res = append_char(*res, '$');
		return (i);
	}
	start = i;
	while (ft_isalnum(str[i]) || str[i] == '_')
		i++;
	var_name = ft_substr(str, start, i - start);
	var_value = get_env_value(sh->env_list, var_name);
	if (var_value)
		*res = append_str(*res, var_value);
	free(var_name);
	return (i);
}

/*
Remove aspas da string final já expandida
Ex: "leo" -> leo
*/
static void	remove_quotes_final(char *str)
{
	int	i;
	int	j;
	int	in_single;
	int	in_double;

	i = 0;
	j = 0;
	in_single = 0;
	in_double = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '\"' && !in_single)
			in_double = !in_double;
		else
			str[j++] = str[i];
		i++;
	}
	str[j] = '\0';
}

/*
Função principal da expansão de uma string.
Lida com quotes para saber quando expandir $.
*/
void	expand_token(t_token *tok, t_shell *sh)
{
	char	*new_str;
	int		i;
	int		in_single;
	int		in_double;

	new_str = ft_strdup("");
	i = 0;
	in_single = 0;
	in_double = 0;
	while (tok->value[i])
	{
		if (tok->value[i] == '\'' && !in_double)
		{
			in_single = !in_single;
			new_str = append_char(new_str, tok->value[i++]);
		}
		else if (tok->value[i] == '\"' && !in_single)
		{
			in_double = !in_double;
			new_str = append_char(new_str, tok->value[i++]);
		}
		else if (tok->value[i] == '$' && !in_single)
			i = handle_dollar(tok->value, i, &new_str, sh);
		else
			new_str = append_char(new_str, tok->value[i++]);
	}
	free(tok->value);
	tok->value = new_str;
	remove_quotes_final(tok->value);
}

void	expand_all_tokens(t_shell *sh, t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == WORD)
			expand_token(tmp, sh);
		tmp = tmp->next;
	}
}