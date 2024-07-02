/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 16:29:15 by ochetrit          #+#    #+#             */
/*   Updated: 2024/07/02 15:43:17 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/minishell.h"

int	is_flag(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
	{
		i++;
		while (str[i] && str[i] == 'n')
			i++;
		if (!str[i])
			return (1);
	}
	return (0);
}

int	skip_flag(char	*str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
	{
		i++;
		while (str[i] && str[i] == 'n')
			i++;
		if (!str[i])
			return (1);
	}
	return (0);
}

void	print_word_echo(char *str)
{
	while (str && *str)
	{
		if (*str != '\\')
			write(1, str, 1);
		if (str)
			str++;
	}
}

void	ft_echo(t_token *tok)
{
	int	flag;
	t_token *lst;
	
	lst = tok;
	if (lst && lst->type == CMD)
		lst = lst->next;
	if (!lst)
		return ;
	flag = is_flag(lst->value);
	while (skip_flag(lst->value))
		lst = lst->next;
	while (lst && lst->type == ARG)
	{
		print_word_echo(lst->value);
		lst = lst->next;
		if (lst && lst->type == ARG)
			write(1, " ", 1);
	}
	if (!flag)
		printf("\n");
}
