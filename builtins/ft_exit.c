/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 09:19:00 by ochetrit          #+#    #+#             */
/*   Updated: 2024/07/04 14:00:01 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/minishell.h"

int		is_long(char *str, int len)
{
	int	i;

	if (len > 19)
		return (FALSE);
	else if (len < 19)
		return (TRUE);
	i = 0;
	if (str[i] == '-')
	{
		i++;
		while (str[i] <= "9223372036854775808"[i] && str[i])
			i++;
	}
	else
	{
		while (str[i] <= "9223372036854775807"[i] && str[i])
			i++;
	}
	if (str[i])
		return (FALSE);
	return (TRUE);
}

int		is_numeric(char *str)
{
	int	i;
	int	neg;

	i = 0;
	neg = 0;
	if (!str)
		return (FALSE);
	else if (str[i] == '-' || str[i] =='+')
	{
		if (str[i] == '-')
			neg = 1;
		i++;
	}
	while (str[i])
	{
		if (str[i] > '9' || str[i] < '0')
			break ;
		i++;
	}
	if (str[i])
		return (FALSE);
	return (is_long(str, i - neg));
}

int		print_msg(t_token **tokens, t_token *lst, t_env **env)
{
	long	code_exit;
	
	printf("exit\n");
	if (!lst)
		return (TRUE);
	if (is_numeric(lst->value))
	{
		code_exit = ft_atoi(lst->value);
		lst = lst->next;
		if (lst && lst->type == ARG)
			return (write(1, "bash: exit: too many arguments\n", 32), FALSE);
		free_everything(env, tokens, code_exit % 255);
	}
	else if (lst->type < PIPE)
	{
		write(2, "bash: exit: ", 12);
		write(2, lst->value, ft_strlen(lst->value));
		write(2, ": numeric argument required\n", 29);
	}
	return (TRUE);
}

void	ft_exit(t_token **tokens, t_token *lst, t_env **env)
{
	close_redirect(tokens);
	if (dup2((*env)->fd_out, STDOUT_FILENO) == -1)
		perror(ERROR_DUP);
	if (lst && lst->type == CMD)
		lst = lst->next;
	if (!print_msg(tokens, lst, env))
		return ;
	if (!lst)
		free_everything(env, tokens, 0);
	free_everything(env, tokens, 128);
}