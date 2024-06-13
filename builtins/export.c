/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 16:08:44 by ochetrit          #+#    #+#             */
/*   Updated: 2024/06/13 13:13:56 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/minishell.h"

void	build_tab(char *value, char *tab[2])
{
	int	len;

	len = 0;
	tab[0] = NULL;
	while (value[len] && value[len] != '=')
		len++;
	if (!value[len])
		return ;
	tab[0] = malloc(sizeof(char) * (len + 1));
	if (!tab[0])
	{
		printf("Erreur malloc\n");
		return ;
	}
	len = -1;
	while (value[++len] != '=')
		tab[0][len] = value[len];
	tab[0][len] = '\0';
	value += len + 1;
	tab[1] = ft_strdup(value);
	if (!tab[1])
	{
		free(tab[0]);
		printf("Erreur malloc\n");
	}
}

int	ft_concatenate_var(char *tab[2], t_env *l_env, t_env **env)
{
	int	i;
	char	*tmp;
	
	i = 0;
	while (tab[0][i])
		i++;
	if (i > 0 && tab[0][i - 1] != '+')
		return (FALSE);
	tab[0][ft_strlen(tab[0]) - 1] = '\0';
	while (l_env && ft_strcmp(l_env->key, tab[0]))
		l_env = l_env->next;
	if (l_env)
	{
		tmp = ft_strjoin(l_env->value, tab[1]);
		free(l_env->value);
		l_env->value = ft_strdup(tmp);
		if (!l_env->value)
			return (printf("Error malloc\n"), TRUE);
		free(tmp);
		return (TRUE);
	}
	if (!ft_create_var(env, tab))
		return (printf("Erreur malloc\n"), TRUE);
	return (TRUE);
}

int	ft_export_bis(t_token *lst, t_env **env)
{
	t_env	*l_env;
	char	*tab[2];

	l_env = *env;
	build_tab(lst->value, tab);
	if (!tab[0] || !tab[1])
		return (FALSE);
	if (ft_concatenate_var(tab, l_env, env))
		return (free(tab[0]), free(tab[1]), TRUE);
	while (l_env && ft_strcmp(l_env->key, tab[0]))
		l_env = l_env->next;
	if (l_env)
	{
		free(l_env->value);
		l_env->value = ft_strdup(tab[1]);
		if (!l_env->value)
			return (printf("Error malloc\n"), free(tab[0]), free(tab[1]), TRUE);
	}
	else
	{
		if (!ft_create_var(env, tab))
			return (printf("Error malloc\n"), free(tab[0]), free(tab[1]), TRUE);
	}
	return (free(tab[0]), free(tab[1]), TRUE);
}

int	ft_export(t_token *tokens, t_env **env)
{
	t_token	*lst;

	lst = tokens;
	if (lst->type == CMD)
		lst = lst->next;
	if (!lst || lst->type != ARG)
		return (print_env_in_order(env), TRUE);
	while (lst && lst->type == ARG)
	{
		if (!ft_export_bis(lst, env))
			return (FALSE);
		lst = lst->next;
	}
	return (TRUE);
}
