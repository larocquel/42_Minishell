/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:51:50 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/02 22:51:51 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_init(t_shell *sh, char **envp)
{
	memset(sh, 0, sizeof(*sh));
	sh->envp = ms_env_dup(envp);
	sh->last_status = 0;
	sh->interactive = 0;
}

void	shell_destroy(t_shell *sh)
{
	ms_free_strarr(sh->envp);
	sh->envp = NULL;
}
