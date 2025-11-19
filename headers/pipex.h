/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thbouver <thbouver@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:50:13 by thbouver          #+#    #+#             */
/*   Updated: 2025/11/19 13:10:58 by thbouver         ###   ########.fr       */
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
}	t_command;

typedef struct s_pipex
{
	t_command	*cmds;
	char		**envp;
	char		*file_in;
	char		*file_out;
	int			total_cmds;
	int			in_fd;
	int			out_fd;
	int			pipe_a[2];
	int			pipe_b[2];
	int			execstatus;
}	t_pipex;


int	parser(t_pipex *pipex, char **argv, char **envp, int argc);

char	*ft_strcat(char *dest, char *src);
void	free_tab(char **tab);
int		count_tab(char **tab);

#endif
