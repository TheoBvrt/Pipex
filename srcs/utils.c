/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theo <theo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:01:27 by thbouver          #+#    #+#             */
/*   Updated: 2025/11/21 10:33:46 by theo             ###   ########.fr       */
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

void	close_all(int **tab, int size)
{
	int	index;

	index = 0;
	while (index < size)
	{
		close (tab[index][1]);
		close(tab[index][0]);
		index ++;
	}
}

int	clean_child(t_pipex *pipex, int exit_value)
{
	close_all(pipex->pipe_tab, pipex->total_cmds - 1);
	free(pipex->cmd);
	clean_exit(pipex);
	return (exit_value);
}
