/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings_to_tokens.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftilliet <ftilliet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 18:22:38 by florian           #+#    #+#             */
/*   Updated: 2024/09/10 18:08:06 by ftilliet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*create_token_node(char *value)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->value = ft_strdup(value);
	new_node->type = get_token_type(value);
	new_node->prev = NULL;
	new_node->next = NULL;
	new_node->file_n = NULL;
	new_node->fd = -1;
	return (new_node);
}

void	add_token_to_list(t_token **token_list, t_token *new_node)
{
	t_token	*current;

	if (*token_list == NULL)
		*token_list = new_node;
	else
	{
		current = *token_list;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
		new_node->prev = current;
	}
}

void	update_flags(t_token *new_node, int *cmd_flag, int *redirection_flag)
{
	if (new_node->type == IN || new_node->type == OUT
		|| new_node->type == APPEND || new_node->type == HEREDOC)
		*redirection_flag = 1;
	else if (new_node->type == ARG)
	{
		if (*cmd_flag == 0 && *redirection_flag == 0)
		{
			new_node->type = CMD;
			*cmd_flag = 1;
		}
		*redirection_flag = 0;
	}
	else if (new_node->type == PIPE)
		*cmd_flag = 0;
}

void	check_env_var(char *str, t_env **env)
{
	int	i;
	int	len;

	len = ft_strlen(str);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			break ;
		if (str[i] == '\"')
			remove_quotes(str, len);
		if (str[i] == '$')
			handle_env_var(str, &i, &len, env);
		i++;
	}
}

t_token	**strings_to_tokens(char **tokens, t_env **env)
{
	t_token	**token_list;
	t_token	*new_node;
	int		cmd_flag;
	int		redirection_flag;
	int		i;

	token_list = malloc(sizeof(t_token *));
	if (!token_list)
		return (NULL);
	*token_list = NULL;
	cmd_flag = 0;
	redirection_flag = 0;
	i = 0;
	while (tokens[i])
	{
		check_env_var(tokens[i], env);
		if (tokens[i][0] == '\0')
		{
			i++;
			continue ;
		}
		new_node = create_token_node(tokens[i]);
		if (!new_node)
			return (NULL);
		update_flags(new_node, &cmd_flag, &redirection_flag);
		add_token_to_list(token_list, new_node);
		i++;
	}
	free_char_tab(tokens);
	return (token_list);
}
