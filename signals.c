/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:13:24 by florian           #+#    #+#             */
/*   Updated: 2024/07/25 12:47:21 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

void	handle_sigint(int code)
{
	(void)code;
	(void)g_exit_code;
	g_exit_code = 1;
	ft_printf("\n", STDOUT_FILENO);
	rl_done = 1;
}

void ctrl_c_here_doc(int code) 
{
	(void)code;
	g_exit_code = 1;
	close(STDIN_FILENO);
	ft_printf("\n", STDOUT_FILENO);
	rl_done = 1;
}

void	signals(int flag)
{
	if (flag == PROMPT) {
		signal(SIGINT, &handle_sigint);
		signal(SIGQUIT, SIG_IGN);
	} else if (flag == HERE_DOC) {
		signal(SIGINT, &ctrl_c_here_doc);
		//signal()
	}
}
