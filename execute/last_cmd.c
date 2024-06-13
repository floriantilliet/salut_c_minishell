/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 17:34:57 by ochetrit          #+#    #+#             */
/*   Updated: 2024/06/13 14:11:59 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/minishell.h"

int	ft_dup_last(t_token *lst)
{
	while (lst && lst->type != PIPE)
	{
		if (lst->type == CMD || lst->type == ARG)
			lst = lst->next;
		else if (lst->type == HEREDOC)
		{
			if (!heredoc(lst->next))
				return (FALSE);
			lst = lst->next;
		}
		else
		{
			if (!ft_redirect(lst->type, lst->next))
				return (FALSE);
			lst = lst->next;
		}
		lst = lst->next;
	}
	return (TRUE);
}

int last_cmd(t_token *lst, t_env **env)
{
    pid_t   pid;
	int		status;

    pid = fork();
    if (pid == -1)
        return (perror(ERROR_FORK), FALSE);
    if (!pid)
    {
		if (!ft_dup_last(lst) || !check_builtins(lst, env))
			free_everything(env, lst->head);
    }
	waitpid(pid, &status, 0);
    return (TRUE);
}
