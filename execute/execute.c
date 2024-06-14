/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:46:29 by ochetrit          #+#    #+#             */
/*   Updated: 2024/06/14 14:59:00 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**initialise_cmd_cmd(t_token *lst, t_cmd *cmd, int len)
{
	int	i;

	cmd->cmd = malloc(sizeof(char *) * (len + 1));
	if (!cmd->cmd)
		return (printf("Erreur malloc\n"), NULL);
	cmd->cmd[len] = NULL;
	i = 0;
	while (lst && i < len)
	{
		if (lst->type < PIPE)
		{
		cmd->cmd[i] = ft_strdup(lst->value);
		lst = lst->next;
		if (!cmd->cmd[i])
			return (free_char_tab(cmd->cmd), free(cmd), printf("Erreur malloc\n"), NULL);
		}
		else
		{
			lst = lst->next;
			lst = lst->next;
		}
		i++;
	}
	return (cmd->cmd);
}

char	*build_env(char *key, char *value)
{
	char	*tmp;
	char	*built;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (printf("Erreur malloc\n"), NULL);
	built = ft_strjoin(tmp, value);
	free(tmp);
	return (built);
}

char	**initialise_cmd_env(t_env **env)
{
	char	**tab;
	int		len;
	t_env	*lst;

	lst = *env;
	len = 0;
	while (lst)
	{
		len++;
		lst = lst->next;
	}
	tab = malloc(sizeof(char *) * (len + 1));
	if (!tab)
		return (printf("Erreur malloc\n"), NULL);
	lst = *env;
	tab[len] = NULL;
	len = -1;
	while(lst && len++ > -2)
	{
		tab[len] = build_env(lst->key, lst->value);
		lst = lst->next;
		if (!tab[len])
			return (printf("Erreur malloc\n"), free_char_tab(tab), NULL);
	}
	return (tab);
}

t_cmd		*initialise_cmd(t_token **tokens, t_env **env)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (printf("Erreur malloc\n"), NULL);
	cmd->free_path = 0;
	cmd->path = NULL;
	cmd->len = 0;
	cmd->is_access = 1;
	cmd->is_pipe = 0;
	cmd->cmd = NULL;
	cmd->env = NULL;
	if (!count_len(tokens, cmd))
		return (free(cmd), NULL);
	if (cmd->len && cmd->is_access)
	{
		cmd->cmd = initialise_cmd_cmd(*tokens, cmd, cmd->len);
		cmd->env = initialise_cmd_env(env);
		if (!cmd->cmd || !cmd->env)
			return (free(cmd), NULL);
	}
	return (cmd);
}

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

int	access_cmd(t_token **tokens, t_env **env)
{
	t_env	*lst_env;
	t_token	*lst_tok;
	t_cmd	*cmd;

	lst_env = *env;
	lst_tok = *tokens;
	cmd = initialise_cmd(tokens, env);
	if (!cmd)
		return (0);
	while (lst_tok->type != CMD && lst_tok->next)
		lst_tok = lst_tok->next;
	if (cmd->is_access && access(lst_tok->value,  F_OK | X_OK) == -1)
		get_access(cmd, lst_tok->value, lst_env);
	else if (access(lst_tok->value,  F_OK | X_OK) != -1)
		cmd->path = ft_strdup(lst_tok->value);
	if (!cmd || !cmd->path || execve(cmd->path, cmd->cmd, cmd->env) == -1)
		printf("Erreur execve\n");
	free_char_tab(cmd->cmd);
	free(cmd->path);
	return (free_char_tab(cmd->env), free(cmd), 0);
}
