/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:46:29 by ochetrit          #+#    #+#             */
/*   Updated: 2024/05/20 20:04:16 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	get_access(t_cmd *cmd, char *str, char *env)
{
	
}

void	access_cmd(t_token **tokens, t_env **env)
{
	t_env	*lst_env;
	t_token	*lst_tok;
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		printf("Erreur malloc");
	cmd->free_path = 0;
	lst_env = *env;
	lst_tok = *tokens;
	while (lst_tok->type != 0 && lst_tok->next)
		lst_tok = lst_tok->next;
	if (access(lst_tok->value,  F_OK | X_OK) != -1)
		printf("oui la commande est carre\n");
	else
	{
		while (lst_env->next && ft_strncmp(lst_env->key, "PATH", 5))
			lst_env = lst_env->next;
		if (!ft_strncmp(lst_env->key, "PATH", 5))
			get_access(cmd, lst_tok->value, lst_env->value);
	}
}
