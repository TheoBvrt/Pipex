/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theo <theo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:01:27 by thbouver          #+#    #+#             */
/*   Updated: 2025/11/20 21:34:52 by theo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_strcat(char *dest, char *src)
{
	char *tmp;

	if (!dest)
	{
		dest = ft_strdup(src);
		if (!dest)
			return (NULL);
		return (dest);
	}
	tmp = ft_strdup(dest);
	if (!tmp)
		return (NULL);
	free (dest);
	dest = ft_strjoin(tmp, src);
	free (tmp);
	return (dest);
}

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
