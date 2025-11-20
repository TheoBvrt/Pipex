/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theo <theo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:01:27 by thbouver          #+#    #+#             */
/*   Updated: 2025/11/21 00:21:05 by theo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	count_tab(char **tab)
{
	int	index;

	index = 0;
	while (tab[index])
		index ++;
	return (index);
}

void	free_tab(char **tab)
{
	int	index;

	index = 0;
	while (tab[index])
	{
		free (tab[index]);
		index ++;
	}
	free(tab);
}

void	free_int_tab(int **tab, int size)
{
	int	index;

	index = 0;
	while (index < size)
		free(tab[index ++]);
	free (tab);
}

void	close_all(t_pipex *pipex, int **tab)
{
	int	index;

	index = 0;
	while (index < (pipex->total_cmds - 1))
	{
		close (tab[index][1]);
		close(tab[index][0]);
		index ++;
	}
}

void	clean_exit(t_pipex *pipex)
{
	int	index;

	index = 0;
	free_int_tab(pipex->pipe_tab, pipex->total_cmds - 1);
	while (index < pipex->total_cmds)
	{
		free_tab(pipex->cmds[index].args);
		index ++;
	}
	free(pipex->cmds);
}
