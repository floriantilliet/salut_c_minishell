/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 17:34:57 by ochetrit          #+#    #+#             */
/*   Updated: 2024/06/14 16:51:33 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/minishell.h"

int	ft_dup_last(t_token *lst, t_env **env)
{
	while (lst)
	{
		if (lst->type == CMD || lst->type == ARG)
			lst = lst->next;
		else if (lst->type == HEREDOC)
		{
			if (!heredoc(lst->next, env))
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
	return (FALSE);
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
		printf("test\n\n\n\n");
		if (!ft_dup_last(lst, env) || !check_builtins(lst, env))
			free_everything(env, lst);
		exit(0);
    }
	waitpid(pid, &status, 0);
    return (TRUE);
}
