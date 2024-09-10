/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_env_var_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftilliet <ftilliet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 17:30:13 by ftilliet          #+#    #+#             */
/*   Updated: 2024/09/10 17:52:29 by ftilliet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	remove_quotes(char *str, int len)
{
	ft_memmove(str, str + 1, len);
	str[len - 2] = '\0';
}

int	find_env_var_length(char *str, int i)
{
	int	j;

	j = 0;
	while (str[i + j + 1] && !is_space(str[i + j + 1]) && str[i + j
			+ 1] != '\"')
		j++;
	return (j);
}

t_env	*find_env_var(t_env *env, char *str, int i, int j)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strncmp(tmp->key, str + i + 1, j - 1) == 0
			&& j == (int)ft_strlen(tmp->key))
			break ;
		tmp = tmp->next;
	}
	return (tmp);
}

void	handle_env_var(char *str, int *i, int *len, t_env **env)
{
	int		j;
	t_env	*tmp;

	j = find_env_var_length(str, *i);
	tmp = find_env_var(*env, str, *i, j);
	if (!tmp)
	{
		ft_memmove(str + *i, str + *i + j + 1, *len - *i - j);
		*len -= j;
		(*i)--;
	}
	else
		*i += j;
}
