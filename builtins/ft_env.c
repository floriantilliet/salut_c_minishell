/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 11:40:47 by florian           #+#    #+#             */
/*   Updated: 2024/07/16 16:14:23 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	printenv(t_env **env, t_token *lst)
{
	t_env	*current;

	current = *env;
	if (lst->next && lst->next->type == ARG)
	{
		ft_printf("env: ‘%s’: No such file or directory\n", 
		STDERR_FILENO, lst->next->value);
		return (exit_status(127, *env), FALSE);
	}
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (exit_status(0, *env), 0);
}

void	exit_status(int code_exit, t_env *env)
{
	env->exit_code = code_exit;
}