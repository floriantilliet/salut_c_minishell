/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 16:20:48 by ochetrit          #+#    #+#             */
/*   Updated: 2024/06/07 15:54:02 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/minishell.h"

void	init_head(t_token **tokens)
{
	t_token *lst;

	lst = *tokens;
	while (lst)
	{
		lst->head = *tokens;
		lst = lst->next;
	}
}

int	check_pipe(t_token **tokens)
{
	t_token *lst;

	lst = *tokens;
	lst->if_pipe = 0;
	init_head(tokens);
	while (lst)
	{
		if (lst->type == PIPE && !lst->next)
			return (printf(ERR_PIPE), FALSE);
		else if (lst->type >= OUT && !lst->next)
			return (printf(ERR_REDIRECT), FALSE);
		else if (lst->type >= OUT && lst->next->type != ARG)
			return (printf(ERR_REDIRECT_2), printf("'%s'\n", lst->next->value), FALSE);
		else if (lst->type == PIPE)
			lst->head->if_pipe++;
		lst = lst->next;
	}
	return (TRUE);
}

int	ft_dup(t_token *lst)
{
	while (lst && lst->type != PIPE)
	{
		if (lst->type == CMD || lst->type == ARG)
			lst = lst->next;
		else if (lst->type == PIPE)
		{
			if (dup2(lst->head->fd_pipe[1], 1) == -1)
				exit(0);
			return (close(lst->head->fd_pipe[1]))
		}
	}
}

int do_cmd(t_token *lst, t_env **env)
{
    pid_t   pid;

    if (pipe(lst->head->fd_pipe) == -1)
        return (ft_printf(ERROR_PIPE) ,FALSE);
    pid = fork();
    if (pid == -1)
        return (ft_printf(ERROR_FORK), FALSE);
    if (!pid)
    {
		close(lst->head->fd_pipe[0]);
		ft_dup(lst);
/*      dup2(pipe_fd[1], 1);
        free_pipe(lst, pipe_fd);
        execve(path_cmd, cmd, envp); */
    }
    else
    {
/*         close(pipe_fd[1]);
        dup2(pipe_fd[0], 0);
        close(pipe_fd[0]); */
    }
    return (TRUE);
}

void	parse_exec(t_token **tokens, t_env **env)
{
	t_token *lst;

	lst = *tokens;
	if (!check_pipe(tokens))
		return ;
	while (lst->head->if_pipe && lst)
	{
		if (!do_cmd(lst, env))
			return ;
		lst->head->if_pipe--;
		while (lst && lst->type != PIPE)
			lst = lst->next;
		if (lst)
			lst = lst->next;
	}
}
