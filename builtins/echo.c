/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 16:29:15 by ochetrit          #+#    #+#             */
/*   Updated: 2024/07/18 11:50:52 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_word_echo(char *str)
{
	while (str && *str)
	{
		if (*str != '\\')
			write(STDOUT_FILENO, str, 1);
		if (str)
			str++;
	}
}

t_token	*skip_redirect(t_token *lst)
{
	if (lst)
		lst = lst->next;
	if (lst)
		lst = lst->next;
	return (lst);
}

int	ft_echo(t_token *lst, t_env **env)
{
	int		flag;
	int	flag2;
	
	if (lst && lst->type == CMD)
		lst = lst->next;
	if (!lst)
		return (write(STDOUT_FILENO, "\n", 1));
	flag = is_flag(lst->value);
	flag2 = 0;
	while (lst && skip_flag(lst->value))
		lst = lst->next;
	while (lst && lst->type != PIPE)
	{
		if (lst->type > PIPE)
    {
			lst = skip_redirect(lst);
			flag2--;
		}
		else
		{
			print_word_echo(lst->value);
			lst = lst->next;
			if (lst && lst->type != PIPE)
				write(STDOUT_FILENO, " ", 1);
			flag2 = 1000;
		}
		if (lst && lst->type == ARG && flag2 > -1)
			write(1, " ", 1);
	}
	if (!flag)
		ft_printf("\n", STDOUT_FILENO);
	return (exit_status(0, *env), TRUE);
}
