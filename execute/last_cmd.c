/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 17:34:57 by ochetrit          #+#    #+#             */
/*   Updated: 2024/07/02 15:56:37 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/minishell.h"

int	ft_dup_last(t_token **tokens, t_token *lst, t_env **env)
{
	while (lst)
	{
		if (lst->type == HEREDOC)
		{
			if (!heredoc(tokens, lst->next, env))
				return (FALSE);
			lst = lst->next;
		}
		else if (lst->type != CMD && lst->type != ARG)
		{
			if (!ft_redirect(lst->type, lst->next))
				return (FALSE);
			lst = lst->next;
		}
		lst = lst->next;
	}
	return (FALSE);
}

void	close_redirect(t_token *lst)
{
	while (lst)
	{
		if (lst->type != CMD && lst->type != ARG)
		{
			lst = lst->next;
			if (!lst)
				return ;
			if (lst->type == ARG)
				close(lst->fd);
		}
		lst = lst->next;
	}
}

int	check_builtins_without_pipe(t_token *lst, t_env **env)
{
	while (lst && lst->type != CMD)
		lst = lst->next;
	if (!lst)
		return (FALSE);
	else if (!ft_strncmp(lst->value, "echo", 5))
		return (ft_echo(lst), TRUE);
	else if (!ft_strncmp(lst->value, "pwd", 4))
		return (ft_pwd(), TRUE);
	else if (!ft_strncmp(lst->value, "cd", 3))
		return (ft_cd(lst), TRUE);
	else if (!ft_strncmp(lst->value, "export", 7))
		return (ft_export(lst, env), TRUE);
	else if (!ft_strncmp(lst->value, "unset", 6))
		return (ft_unset(lst, env), TRUE);
	else if (!ft_strncmp(lst->value, "env", 4))
		return (printenv(env), TRUE);
	else if (!ft_strncmp(lst->value, "exit", 5))
		return (TRUE);
	return (FALSE);
}

int last_cmd(t_token **tokens, t_token *lst, t_env **env)
{
    pid_t   pid;
	int		status;

	ft_dup_last(tokens, lst, env);
	if (check_builtins_without_pipe(lst, env))
		return (close_redirect(lst), TRUE);
    pid = fork();
	if (pid == -1)
        return (perror(ERROR_FORK), FALSE);
    if (!pid)
    {
		printf("test\n\n");
		ft_dup_last(tokens, lst, env);
		if (!check_builtins(lst, env))
			free_everything(env, tokens);
		exit(0);
    }
	waitpid(pid, &status, 0);
    return (TRUE);
}
