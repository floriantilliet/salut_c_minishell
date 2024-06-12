/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <ochetrit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 05:43:39 by ftilliet          #+#    #+#             */
/*   Updated: 2024/06/11 17:36:02 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <errno.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define CMD 0
# define ARG 1
# define PIPE 2
# define OUT 3
# define IN 4
# define APPEND 5
# define HEREDOC 6

# define FALSE 0
# define TRUE 1
# define ERR_PIPE "bash: syntax error near unexpected token `|'\n"
# define ERR_REDIRECT "bash: syntax error near unexpected token `newline'\n"
# define ERR_REDIRECT_2 "bash: syntax error near unexpected token "
# define ERROR_PIPE "Some mistakes occures when using pipe\n"
# define ERROR_DUP "Some mistakes occures when using dup\n"
# define ERROR_FORK "Some mistakes occures when using fork\n"
#  define ERROR_EXEC "Some mistakes occures when using execve\n"

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
	struct s_env	*prev;
}					t_env;

typedef struct s_token
{
	char			*value;
	int				type;
	int				if_pipe;
	int				fd_pipe[2];
	struct s_token	*next;
	struct s_token	*prev;
	struct s_token	*head;
}					t_token;

typedef struct s_cmd
{
	char	*path;
	int		free_path;
	int		len;
	char	**cmd;
	char	**env;
	int		is_pipe;
	int		is_access;
}					t_cmd;
// env.c

t_env				*create_env_node(char *env_str);
void				append_env_node(t_env **env, t_env *new_node);
t_env				**store_env(char **envp);
int					printenv(t_env **env);
char				*get_env_value(char *key, t_env **env);

// expander.c

char				*expander(char *line, t_env **env);

// lexer.c

int					is_space(char c);
char				**line_to_strings(char *line);
void				print_tokens(char **tokens);
int					check_quote_problems(char *line);
int					count_tokens(char *line);
t_token				**strings_to_tokens(char **tokens);
void				print_token_list(t_token **token_list);
int					get_token_type(char *token);
void				expand_token(t_token *current, t_env **env);
void				trim_token(t_token *current);
void				expand_token_list(t_token **token_list, t_env **env);

// cleaning_utils.c

void				free_char_tab(char **tab);
void				free_env(t_env **env);
void				free_token_list(t_token **token_list);
void				free_everything(t_env **env, t_token **tokens);

// signals.c

void				signals(void);
void				sigint_handler(int code);

// builtins

int					ft_pwd(void);
int					ft_cd(t_token **tokens);
int					ft_export(t_token **tokens, t_env **env);
int					ft_create_var(t_env **env, char *tab[2]);
void				ft_echo(t_token **tok);
void				print_env_in_order(t_env **env);
void				ft_unset(t_token **token, t_env **env);
t_env				*ft_lstnew_env(char *key, char *value);

// execute.c

int		access_cmd(t_token **tokens, t_env **env);
int		count_len(t_token **tokens, t_cmd *cmd);
void	parse_exec(t_token **tokens, t_env **env);
int		ft_strcmp(const char *s1, const char *s2);
char	**initialise_cmd_env(t_env **env);
int		do_cmd(t_token *lst, t_env **env);


#endif