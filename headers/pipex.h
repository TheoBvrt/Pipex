/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thbouver <thbouver@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:50:13 by thbouver          #+#    #+#             */
/*   Updated: 2025/11/14 13:35:54 by thbouver         ###   ########.fr       */
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

typedef struct s_pipex
{
	int	fd_file_in;
	int	fd_file_out;
}	t_pipex;

char	*ft_strcat(char *dest, char *src);
void	free_tab(char **tab);

#endif
