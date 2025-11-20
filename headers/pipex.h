/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theo <theo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:50:13 by thbouver          #+#    #+#             */
/*   Updated: 2025/11/21 00:22:12 by theo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
# include "stdio.h"
# include "errno.h"
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef	struct s_command
{
	char	*cmd;
	char	**args;
	int		pid;
}	t_command;

typedef struct s_pipex
{
	t_command	*cmds;
	int	**pipe_tab;
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


int	parser(t_pipex *pipex, char **argv, char **envp, int argc);
char *find_path(char *cmd, char *envp[], int *status);
char	*get_path(char *cmd, char *path);
void	free_int_tab(int **tab, int size);
char	*ft_strcat(char *dest, char *src);
void	free_tab(char **tab);
int		count_tab(char **tab);
void	close_all(t_pipex *pipex, int **tab);
void	clean_exit(t_pipex *pipex);

#endif
