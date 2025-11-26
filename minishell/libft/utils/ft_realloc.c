/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davmendo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 21:44:31 by davmendo          #+#    #+#             */
/*   Updated: 2025/11/26 21:44:51 by davmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *ptr, size_t old_count, size_t new_count, size_t size)
{
	void	*newptr;

	newptr = malloc(new_count * size);
	if (!newptr)
		return (NULL);
	if (ptr && old_count > 0)
	{
		ft_memcpy(newptr, ptr, old_count * size);
		free(ptr);
	}
	return (newptr);
}
