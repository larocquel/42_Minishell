/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoaguia <leoaguia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:52:52 by leoaguia          #+#    #+#             */
/*   Updated: 2025/11/16 19:20:30 by leoaguia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_export(t_ms *sh, char **argv)
{
	int		i;
	int		rc;
	char	*eq;

	i = 1;
	rc = 0;
	if (!argv[1]) return (builtin_env(sh));
	while (argv[i])
	{
		eq = strchr(argv[i], '=');
		if (!eq || eq == argv[i])
			rc = fprintf(stderr, "minishell: export: `%s': not a valid identifier\n",
				argv[i]) || rc;
		else
		{
			*eq = '\0';
			ms_setenv(sh, argv[i], eq + 1);
			*eq = '=';
		}
		i++;
	}
	return (rc ? 1 : 0);
}
