/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:46:29 by ochetrit          #+#    #+#             */
/*   Updated: 2024/07/26 13:45:19 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	get_access(t_cmd *cmd, char *str, t_env *lst_env)
{
	char	**tab;
	char	*tmp;
	int		i;

	while (lst_env->next && ft_strcmp(lst_env->key, "PATH"))
		lst_env = lst_env->next;
	if (ft_strcmp(lst_env->key, "PATH"))
		return ;
	i = -1;
	tab = ft_split(lst_env->value, ':');
	while (tab[++i])
	{
		tmp = ft_strjoin(tab[i], "/");
		cmd->path = ft_strjoin(tmp, str);
		free(tmp);
		if (access(cmd->path, F_OK | X_OK) != -1)
		{
			free_char_tab(tab);
			cmd->free_path = 1;
			return ;
		}
		free(cmd->path);
	}
	free_char_tab(tab);
	cmd->path = NULL;
}

void	path_cmd(char *value, t_cmd *cmd, t_env **env, t_token **tokens)
{
	if (access(value, F_OK) == -1)
		get_access(cmd, value, *env);
	else
		cmd->path = ft_strdup(value);
	(void)tokens;
	/* if (access(cmd->path, X_OK) == -1 && ft_strchr(value, '/') && errno == EACCES)
	{
		free_char_tab(cmd->cmd);
		free_char_tab(cmd->env);
		free(cmd->path);
		free(cmd);
		ft_printf(ERROR_PERMISSION, STDERR_FILENO, value);
		free_everything(env, tokens, 126);
	} */
}

int	access_cmd(t_token *lst, t_token **tokens, t_env **env)
{
	t_cmd	*cmd;

	cmd = initialise_cmd(&lst, env);
	if (!cmd)
		return (0);
	while (lst->type != CMD && lst->next)
		lst = lst->next;
	close_redirect(tokens);
	close_dup(env);
	path_cmd(lst->value, cmd, env, tokens);
	if (!cmd || !cmd->path || execve(cmd->path, cmd->cmd, cmd->env) == -1)
		ft_printf("Error execve\n", STDERR_FILENO);
	free_char_tab(cmd->cmd);
	free_char_tab(cmd->env);
	free(cmd->path);
	free(cmd);
	if (errno == ENOENT)
		free_everything(env, tokens, 127);
	return (free_everything(env, tokens, errno), 0);
}
