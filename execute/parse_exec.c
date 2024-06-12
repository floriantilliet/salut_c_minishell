/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 16:20:48 by ochetrit          #+#    #+#             */
/*   Updated: 2024/06/11 17:36:19 by ochetrit         ###   ########.fr       */
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

int do_last_cmd(t_token *lst, t_env **env)
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
        dup2(lst->head->fd_pipe[0], 0);
        close(lst->head->fd_pipe[0]);
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
	do_last_cmd(lst, env);
	return ;
}
