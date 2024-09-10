/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_types.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftilliet <ftilliet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 17:44:16 by florian           #+#    #+#             */
/*   Updated: 2024/09/10 14:36:35 by ftilliet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_token_chars(char *token, int len)
{
	if (len == 1)
	{
		if (token[0] == '|')
			return (PIPE);
		if (token[0] == '>')
			return (OUT);
		if (token[0] == '<')
			return (IN);
	}
	else if (len == 2)
	{
		if (token[0] == '>' && token[1] == '>')
			return (APPEND);
		if (token[0] == '<' && token[1] == '<')
			return (HEREDOC);
	}
	return (ARG);
}

int	get_token_type(char *token)
{
	int	len;
	int i;

	i = 0;
	len = ft_strlen(token);
	// if (is_space(token[0]))
	// 	return (IS_SPACE);
	while (is_space(token[i]) && token[i] && i < len)
		i++;
	if (i == len)
		return (IS_SPACE);
	else
		return (check_token_chars(token, len));
}
