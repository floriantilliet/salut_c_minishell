/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 13:08:16 by ochetrit          #+#    #+#             */
/*   Updated: 2024/06/03 15:21:17 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/minishell.h"

int		ft_cd(t_token **tokens)
{
	t_token *lst;
	char	*pwd;
	char	*str;
	
	lst = *tokens;
	while (lst && lst->type != CMD)
		lst = lst->next;
	if (!lst && !lst->next && (lst->next)->type == ARG)
		return(perror("cd need args\n"), FALSE);
	lst = lst->next;
	if (chdir(lst->value) == 0)
		return (TRUE);
	pwd = getcwd(NULL, 0);
	str = ft_strjoin(pwd, lst->value);
	free(pwd);
	if (chdir(str) != 0)
		return (free(str), perror("cd"), FALSE);
	free(str);
	return (TRUE);
}
