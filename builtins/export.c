/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 16:08:44 by ochetrit          #+#    #+#             */
/*   Updated: 2024/07/05 17:25:31 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/minishell.h"

void	build_tab2(char *value, char *tab[2], int len, t_env *env)
{
	tab[0] = malloc(sizeof(char) * (len + 1));
	if (!tab[0])
	{
		perror("Erreur malloc\n");
		exit_status(1, env);
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
		perror("Erreur malloc\n");
		exit_status(1, env);
	}
}

void	build_tab(char *value, char *tab[2], t_env *env)
{
	int	len;
	t_env	*lst;

	len = 0;
	lst = 	env;
	tab[0] = NULL;
	tab[1] = NULL;
	while (value[len] && value[len] != '=')
		len++;
	if (!value[len])
	{
		while (lst && lst->next)
			lst = lst->next;
		lst->next = ft_lstnew_env(value, NULL, TRUE);
		exit_status(0, env);
		return ;
	}
	build_tab2(value, tab, len, env);
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
			return (printf("Error malloc\n"), exit_status(1, *env), TRUE);
		free(tmp);
		return (exit_status(0, *env), TRUE);
	}
	if (!ft_create_var(env, tab))
		return (printf("Erreur malloc\n"), exit_status(1, *env), TRUE);
	return (exit_status(0, *env), TRUE);
}

int	ft_export_bis(t_token *lst, t_env **env)
{
	t_env	*l_env;
	char	*tab[2];

	l_env = *env;
	build_tab(lst->value, tab, *env);
	if (!tab[0] && !tab[1])
		return (TRUE);
	if (ft_concatenate_var(tab, l_env, env))
		return (free(tab[0]), free(tab[1]), TRUE);
	while (l_env && ft_strcmp(l_env->key, tab[0]))
		l_env = l_env->next;
	if (l_env)
	{
		free(l_env->value);
		l_env->value = ft_strdup(tab[1]);
		exit_status(1, *env);
		if (!l_env->value)
			return (perror(ERR_MALLOC), free(tab[0]), free(tab[1]), TRUE);
	}
	else
	{
		if (!ft_create_var(env, tab))
			return (perror(ERR_MALLOC), free(tab[0]), free(tab[1]), TRUE);
	}
	return (free(tab[0]), free(tab[1]), exit_status(0, *env), TRUE);
}

int	parsing_export(char *str)
{
	if (*str != '_' && !ft_isalpha(*str) && *str)
	{
		ft_printf("bash: export: `%s': not a valid identifier\n", 2, str);
		return (FALSE);
	}
	str++;
	while (*str)
	{
		if (*str == '=')
			break ;
		else if (*str != '_' && !ft_isalnum(*str))
		{
			ft_printf("bash: export: `%s': not a valid identifier\n", 2, str);
			return (FALSE);
		}
		str++;
	}
	return (TRUE);
}

int	ft_export(t_token *tokens, t_env **env)
{
	t_token		*lst;
	int			exit_code;

	lst = tokens;
	exit_code = 0;
	if (lst->type == CMD)
		lst = lst->next;
	if (!lst || lst->type != ARG)
		return (print_env_in_order(env), exit_status(0, *env), TRUE);
	if (ft_strchr(lst->value, '-') + 1 == ft_strchr(lst->value, '='))
	{
		ft_printf("bash: export: `%s': not a valid identifier\n", 2, lst->value);
		return (exit_status(1, *env), FALSE);
	}
	while (lst && lst->type == ARG)
	{
		if (!parsing_export(lst->value))
			exit_code = 1;
		else if (!ft_export_bis(lst, env))
			return (FALSE);
		lst = lst->next;
	}
	if (exit_code)
		exit_status(exit_code, *env);
	return (TRUE);
}
