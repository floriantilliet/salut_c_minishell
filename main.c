/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 05:41:13 by ftilliet          #+#    #+#             */
/*   Updated: 2024/07/05 15:34:18 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

int	init_std(t_env *env)
{
	if (dup2(env->fd_in, STDIN_FILENO) == -1)
		return (printf(ERROR_DUP), FALSE);
	if (dup2(env->fd_out, STDOUT_FILENO) == -1)
		return (printf(ERROR_DUP), FALSE);
	return (TRUE);
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
		printf("No environment variables found\n");
		return (1);
	}
	env = NULL;
	env = store_env(envp);
	signals();
	(*env)->fd_in = dup(STDIN_FILENO);
	(*env)->fd_out = dup(STDOUT_FILENO);
	line = "\0";
	//exit_status(0, *env);
	while (line != NULL)
	{
		line = readline("minishell $> ");
		if (line)
		{
			add_history(line);
			// printf("%s\n", expander(line, env));
			if (!check_problems(line))
				;				
			else
			{
				tokens = (merge_tokens(strings_to_tokens(line_to_strings(line))));
				expand_token_list(tokens, env);
				parse_exec(tokens, env);
				// print_token_list(tokens);
				/* if (*tokens)
					access_cmd(tokens, env); */
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

