/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 11:40:03 by ochetrit          #+#    #+#             */
/*   Updated: 2024/07/03 17:10:32 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/minishell.h"

int		ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (FALSE);
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s2 - *s1);
}

void	put_env_in_order(char **tab)
{
	int	i;
	int	j;
	char	*tmp;

	i = 0;
	while (tab[i])
	{
		j = i + 1;
		while (tab[j])
		{
			if (ft_strcmp(tab[i], tab[j]) < 0)
			{
				tmp = tab[i];
				tab[i] = tab[j];
				tab[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_env_in_order(t_env **env)
{
	char	**tab;

	tab = initialise_cmd_env(env);
	if (!tab)
	{
		printf("ERREUR MALLOC\n");
		return ;
	}
	put_env_in_order(tab);
	int i = 0;
	while (tab[i])
	{
		printf("declare -x %s\n", tab[i]);
		i++;
	}
	free_char_tab(tab);
}

t_env	*ft_lstnew_env(char *key, char *value, int create)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	if (!new->key)
		return (free(new), NULL);
	if (create)
	{
		new->value = NULL;
		new->next = NULL;
		return (new);
	}
	new->value = ft_strdup(value);
	if (!new->value)
		return (free(new->key), free(new), NULL);
	new->next = NULL;
	return (new);
}

int	ft_create_var(t_env **env, char *tab[2])
{
	t_env	*lst;

	lst = *env;
	while (lst->next)
		lst = lst->next;
	lst->next = ft_lstnew_env(tab[0], tab[1], FALSE);
	if (!lst->next)
		return (printf("Error malloc\n"), FALSE);
	return (TRUE);
}
