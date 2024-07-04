/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 13:08:16 by ochetrit          #+#    #+#             */
/*   Updated: 2024/07/03 15:48:37 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/minishell.h"

int		cd_home(t_env *env)
{
	while (env)
	{
		if (!ft_strcmp(env->key, "HOME"))
			break ;
		env = env->next;
	}
	if (!env)
		return (ft_printf("bash: cd: HOME not set\n", 2), FALSE);
	if (chdir(env->value) != 0)
		return (ft_printf("bash: cd: %s: No such file or directory\n", 2, 
		env->value), FALSE);
	return (TRUE);
}

int		ft_cd(t_token *tokens, t_env **env)
{
	t_token *lst;
	char	*pwd;
	char	*str;
	
	lst = tokens;
	while (lst && lst->type != CMD)
		lst = lst->next;
	lst = lst->next;
	if (!lst || lst->type != ARG)
		return(cd_home(*env));
	if (chdir(lst->value) == 0)
		return (TRUE);
	pwd = getcwd(NULL, 0);
	str = ft_strjoin(pwd, lst->value);
	free(pwd);
	if (chdir(str) != 0)
		return (free(str), ft_printf("bash: cd: %s: No such file or directory\n", 2,
		 lst->value), FALSE);
	free(str);
	return (TRUE);
}
