/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theo <theo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 10:33:11 by theo              #+#    #+#             */
/*   Updated: 2025/11/21 10:43:13 by theo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	clean_exit(t_pipex *pipex)
{
	int	index;

	index = 0;
	while (index < pipex->total_cmds)
	{
		free_tab(pipex->cmds[index].args);
		index ++;
	}
	free(pipex->cmds);
	index = 0;
	while (index < pipex->total_cmds - 1)
		free(pipex->pipe_tab[index ++]);
	free (pipex->pipe_tab);
}

void	fail_init_exit(t_pipex *pipex, int close_fd, int nb_pipes, int tab_s)
{
	int	i;

	i = 0;
	if (close_fd)
		close_all(pipex->pipe_tab, nb_pipes);
	while (i < pipex->total_cmds)
	{
		free_tab(pipex->cmds[i].args);
		i ++;
	}
	free(pipex->cmds);
	i = 0;
	while (i < tab_s)
		free(pipex->pipe_tab[i ++]);
	free (pipex->pipe_tab);
}
