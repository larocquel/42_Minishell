/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 14:17:00 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/05 23:03:51 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*lex_line(const char *s, int *err)
{
	size_t			i;
	t_token			*head;
	t_token			*tail;
	t_token			*node;
	char			*val;
	unsigned char	*mask;

	if (!s || !err)
		return (NULL);
	*err = 0;
	head = NULL;
	tail = NULL;
	i = 0;
	while (s[i])
	{
		while (s[i] && tk_is_space(s[i]))
			i++;
		if (!s[i])
			break ;
		if (tk_is_operator(s[i]))
		{
			node = tk_read_op(s, &i);
			if (!node)
				return (free_token_list(head), NULL);
		}
		else
		{
			val = tk_read_word(s, &i, err, &mask);
			if (*err || !val)
				return (free_token_list(head), NULL);
			node = tk_new(WORD, val, mask);
			if (!node)
				return (free(val), free(mask), free_token_list(head), NULL);
		}
		tk_push(&head, &tail, node);
	}
	return (head);
}

void	free_token_list(t_token *lst)
{
	t_token	*n;

	while (lst)
	{
		n = lst->next;
		free(lst->val);
		free(lst->qmask);
		free(lst);
		lst = n;
	}
}
