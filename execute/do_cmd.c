/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 17:34:38 by ochetrit          #+#    #+#             */
/*   Updated: 2024/06/11 17:35:41 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/minishell.h"

int	ft_dup(t_token *lst)
{
	close(lst->head->fd_pipe[0]);
	if (dup2(lst->head->fd_pipe[1], 1) == -1)
		return (FALSE);
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
	return (close(lst->head->fd_pipe[1]), TRUE);
}

int	check_builtins(t_token *lst, t_env **env)
{
	while (lst && lst->type != CMD)
		lst = lst->next;
	if (!lst)
		return (FALSE);
	else if (!ft_strncmp(lst->value, "echo", 5))
		return (ft_echo(lst), FALSE);
	else if (!ft_strncmp(lst->value, "pwd", 4))
		return (ft_pwd, FALSE);
	else if (!ft_strncmp(lst->value, "cd", 3))
		return (ft_cd(lst), FALSE);
	else if (!ft_strncmp(lst->value, "export", 7))
		return (ft_export(lst, env), FALSE);
	else if (!ft_strncmp(lst->value, "unset", 6))
		return (ft_unset(lst, env), FALSE);
	else if (!ft_strncmp(lst->value, "env", 4))
		return (printenv(lst), FALSE);
	else if (!ft_strncmp(lst->value, "exit", 5))
		return (FALSE);
	else
		access_cmd(&lst, env);
	return (FALSE);
}

int do_cmd(t_token *lst, t_env **env)
{
    pid_t   pid;

    if (pipe(lst->head->fd_pipe) == -1)
        return (perror(ERROR_PIPE) ,FALSE);
    pid = fork();
    if (pid == -1)
        return (perror(ERROR_FORK), FALSE);
    if (!pid)
    {
		if (!ft_dup(lst) || !check_builtins(lst, env))
			free_everything(env, lst->head);
    }
    else
    {
        close(lst->head->fd_pipe[1]);
    	if (dup2(lst->head->fd_pipe[0], 0) == -1)
			perror(ERROR_DUP);
        close(lst->head->fd_pipe[0]);
    }
    return (TRUE);
}