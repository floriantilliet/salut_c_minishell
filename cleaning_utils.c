/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 12:14:26 by florian           #+#    #+#             */
/*   Updated: 2024/07/03 17:22:14 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

void	free_char_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	free_env(t_env **env)
{
	t_env	*current;
	t_env	*next;

	current = *env;
	while (current)
	{
		next = current->next;
		free(current->key);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
	free(env);
}

void	free_token_list(t_token **token_list)
{
	t_token	*current;
	t_token	*next;

	current = *token_list;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	free(token_list);
}

void	free_everything(t_env **env, t_token **tokens, int code_exit)
{
	if (tokens)
		free_token_list(tokens);
	if ((*env)->fd_in)
		close((*env)->fd_in);
	if ((*env)->fd_out)
		close((*env)->fd_out);
	if (env)
		free_env(env);
	exit (code_exit);
}
