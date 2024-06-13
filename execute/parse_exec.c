/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 16:20:48 by ochetrit          #+#    #+#             */
/*   Updated: 2024/06/13 14:46:01 by ochetrit         ###   ########.fr       */
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

int	fork_one_cmd(t_token *lst, t_env **env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	 if (pid == -1)
        return (perror(ERROR_FORK), FALSE);
	if (!pid)
	{
		access_cmd(&lst, env);
		free_everything(env, lst);
	}
	waitpid(pid, &status, 0);
	return (TRUE);
}


int	check_builtins_without_pipe(t_token *lst, t_env **env)
{
	while (lst && lst->type != CMD)
		lst = lst->next;
	if (!lst)
		return (FALSE);
	else if (!ft_strncmp(lst->value, "echo", 5))
		return (ft_echo(lst), FALSE);
	else if (!ft_strncmp(lst->value, "pwd", 4))
		return (ft_pwd(), FALSE);
	else if (!ft_strncmp(lst->value, "cd", 3))
		return (ft_cd(lst), FALSE);
	else if (!ft_strncmp(lst->value, "export", 7))
		return (ft_export(lst, env), FALSE);
	else if (!ft_strncmp(lst->value, "unset", 6))
		return (ft_unset(lst, env), FALSE);
	else if (!ft_strncmp(lst->value, "env", 4))
		return (printenv(env), FALSE);
	else if (!ft_strncmp(lst->value, "exit", 5))
		return (FALSE);
	else
		fork_one_cmd(lst, env);
	return (FALSE);
}

void	parse_exec(t_token **tokens, t_env **env)
{
	t_token *lst;

	lst = *tokens;
	if (!check_pipe(tokens))
		return ;
	if (!lst->head->if_pipe)
	{
		check_builtins_without_pipe(lst, env);
		return ;
	}
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
	last_cmd(lst, env);
	return ;
}
