/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davmendo <davmendo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:32:42 by davmendo          #+#    #+#             */
/*   Updated: 2025/11/13 16:38:28 by davmendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ms_strarr_sort(char **arr)
{
	size_t	i;
	size_t	j;
	char	*tmp;

	if (arr == NULL)
		return ;
	i = 0;
	while (arr[i] != NULL)
	{
		j = i + 1;
		while (arr[j] != NULL)
		{
			if (strcmp(arr[i], arr[j]) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	ms_export_print_one(const char *entry)
{
	size_t	i;

	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	i = 0;
	while (entry[i] != '\0' && entry[i] != '=')
	{
		ft_putchar_fd(entry[i], STDOUT_FILENO);
		i++;
	}
	if (entry[i] == '=')
	{
		i++;
		ft_putstr_fd("=\"", STDOUT_FILENO);
		while (entry[i] != '\0')
		{
			ft_putchar_fd(entry[i], STDOUT_FILENO);
			i++;
		}
		ft_putchar_fd('"', STDOUT_FILENO);
	}
	ft_putchar_fd('\n', STDOUT_FILENO);
}

/*
 * export
 * - Lista todas as entradas de sh->envp, ordenadas,
 *   no formato "declare -x NAME" ou "declare -x NAME=\"VAL\"".
 */
int	ms_export_no_args(t_shell *sh)
{
	char	**copy;
	size_t	i;

	if (sh == NULL)
		return (1);
	copy = ms_env_dup(sh->envp);
	if (copy == NULL)
		return (1);
	ms_strarr_sort(copy);
	i = 0;
	while (copy[i] != NULL)
	{
		ms_export_print_one(copy[i]);
		i++;
	}
	ms_free_strarr(copy);
	return (0);
}
