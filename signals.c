/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:13:24 by florian           #+#    #+#             */
/*   Updated: 2024/07/16 16:10:00 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

void	handle_sigint(int code)
{
	(void)code;
	(void)g_exit_code;
	g_exit_code = 1;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	signals(void)
{
	signal(SIGINT, &handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
