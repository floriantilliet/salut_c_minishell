/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 15:11:05 by ochetrit          #+#    #+#             */
/*   Updated: 2024/06/13 12:28:03 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		ft_redirect(int type, t_token *lst)
{
	int	fd;

	if (!lst || lst->type != ARG)
		return (FALSE);
	if (type == IN)
	{
		fd = open(lst->value, O_RDONLY);
		if (fd == -1)
			return (perror("Erreur open\n"), FALSE);
		if (dup2(fd, 0) == -1)
			return (perror("Erreur dup2"), FALSE);
	}
	else
	{
		if (type == APPEND)
			fd = open(lst->value, O_WRONLY | O_CREAT | O_APPEND, 0000644);
		else
			fd = open(lst->value, O_CREAT | O_RDWR | O_TRUNC, 0000644);
		if (fd == -1)
			return (perror("Erreur open\n"), FALSE);
		if (dup2(fd, 1) == -1)
			return (perror("Erreur dup2"), FALSE);
	}
	return (TRUE);
}

void    here_doc_put_in(char *limit, int pipe_fd[2])
{
    char    *ret;

    close(pipe_fd[0]);
    while (1)
    {
        ret = get_next_line(0);
        if (!ft_strncmp(ret, limit, ft_strlen(limit)))
        {
            close(pipe_fd[1]);
            free(ret);
            exit(0);
        }
        ft_putstr_fd(ret, pipe_fd[1]);
        free(ret);
    }
}

int    heredoc(t_token *lst)
{
    int		    pipe_fd[2];
	pid_t	pid;

    if (pipe(pipe_fd) == -1)
        return (perror("ERROR_PIPE"), FALSE);
    pid = fork();
    if (pid == -1)
        return (perror("ERROR_FORK"), FALSE);
    if (!pid)
		here_doc_put_in(lst->value, pipe_fd);
	else
	{
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], 0) == -1)
            return (perror("ERROR_DUP"), FALSE);
        close(pipe_fd[0]);
		wait(NULL);
	}
    return (TRUE);
}

int		ft_is_builtin(int type, char *str, t_cmd *cmd)
{
	if (type != CMD)
		return (FALSE);
	if (!ft_strncmp(str, "echo", 5))
		return (cmd->is_access = 0, TRUE);
	if (!ft_strncmp(str, "pwd", 4))
		return (cmd->is_access = 0, TRUE);
	if (!ft_strncmp(str, "cd", 3))
		return (cmd->is_access = 0, TRUE);
	if (!ft_strncmp(str, "export", 7))
		return (cmd->is_access = 0, TRUE);
	if (!ft_strncmp(str, "unset", 6))
		return (cmd->is_access = 0, TRUE);
	if (!ft_strncmp(str, "env", 4))
		return (cmd->is_access = 0, TRUE);
	if (!ft_strncmp(str, "exit", 5))
		return (cmd->is_access = 0, TRUE);
	return (TRUE);
}

int		count_len(t_token **tokens, t_cmd *cmd)
{
	t_token *lst;

	lst = *tokens;
	cmd->len = 0;
	while (lst)
	{
		if (ft_is_builtin(lst->type, lst->value, cmd))
			cmd->len = 1;
		else if (lst->type == ARG)
			cmd->len++;
		else if (lst->type == PIPE )
			return (cmd->is_pipe = 1, TRUE);
		else if (lst->type == OUT || lst->type == APPEND || lst->type == IN)
		{
			if (!ft_redirect(lst->type, lst->next))
				return (FALSE);
			lst = lst->next;
		}
		else
		{
			if (!heredoc(lst->next))
				return (FALSE);
			lst = lst->next;
		}
		lst = lst->next;
	}
	return (TRUE);
}
