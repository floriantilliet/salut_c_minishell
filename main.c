/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 05:41:13 by ftilliet          #+#    #+#             */
/*   Updated: 2024/07/24 16:40:28 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

int g_exit_code = 0;

int	init_std(t_env *env)
{
	if (dup2(env->fd_in, STDIN_FILENO) == -1)
		return (ft_printf(ERROR_DUP, STDERR_FILENO), FALSE);
	if (dup2(env->fd_out, STDOUT_FILENO) == -1)
		return (ft_printf(ERROR_DUP, STDERR_FILENO), FALSE);
	return (TRUE);
}

static int rl_resync(void)
{
	return (1);
}


int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_env	**env;
	t_token	**tokens;

	(void)av;
	(void)ac;
	if (!*envp || !envp)
	{
		ft_printf("No environment variables found\n", 2);
		return (1);
	}
	env = NULL;
	env = store_env(envp);
	signals();
	(*env)->fd_in = dup(STDIN_FILENO);
	(*env)->fd_out = dup(STDOUT_FILENO);
	line = "\0";
	exit_status(0, *env);
	while (line != NULL)
	{
		rl_event_hook = rl_resync;
		if (g_exit_code != 0)
		{
			exit_status(130, *env);
			g_exit_code = 0;
		}
		line = readline("minishell $> ");
		if (line)
		{
			add_history(line);
			// printf("%s\n", expander(line, env));
			if (!check_problems(line, env))
				;				
			else
			{
				tokens = (merge_tokens(strings_to_tokens(line_to_strings(line))));
				expand_token_list(tokens, env);
				parse_exec(tokens, env);
				free_token_list(tokens);
				init_std(*env);
			}
		}
		free(line);
	}
	rl_clear_history();
	free_env(env);
	return (0);
}

