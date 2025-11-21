/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thbouver <thbouver@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:50:13 by thbouver          #+#    #+#             */
/*   Updated: 2025/11/21 15:49:54 by thbouver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# ifndef BOOLEAN
#  define TRUE 1
#  define FALSE 0
# endif

# include "../libft/libft.h"
# include "stdio.h"
# include "errno.h"
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_command
{
	char	*cmd;
	char	**args;
	int		pid;
}	t_command;

typedef struct s_pipex
{
	t_command	*cmds;
	int			**pipe_tab;
	char		**envp;
	char		*file_in;
	char		*file_out;
	char		*cmd;
	int			total_cmds;
	int			in_fd;
	int			out_fd;
	int			pipe_a[2];
	int			pipe_b[2];
}	t_pipex;

char	*find_path(char *cmd, char *envp[], int *status);

void	fail_init_exit(t_pipex *pipex, int close_fd, int nb_pipes, int tab_s);
void	close_all(int **tab, int size);
void	clean_exit(t_pipex *pipex);
void	free_tab(char **tab);

int		parser(t_pipex *pipex, char **argv, char **envp, int argc);
int		run_pipeline(t_pipex *pipex);
int		clean_child(t_pipex *pipex, int exit_value);
int		count_tab(char **tab);
int		is_dir(char *path);

#endif
