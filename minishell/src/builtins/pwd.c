/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:52:56 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/02 22:52:57 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(void)
{
	char	buf[4096];

	if (!getcwd(buf, sizeof(buf))) return (perror("pwd"), 1);
	printf("%s\n", buf);
	return (0);
}
