/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 16:08:44 by ochetrit          #+#    #+#             */
/*   Updated: 2024/06/04 17:08:56 by ochetrit         ###   ########.fr       */
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

int	ft_export(t_token **tokens, t_env **env)
{
	t_token	*lst;

	lst = *tokens;
	if (lst->type == CMD)
		lst = lst->next;
	if (!lst || lst->type != ARG)
		print_env_in_order(env);
	return (TRUE);
}