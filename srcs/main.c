/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theo <theo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:46:18 by thbouver          #+#    #+#             */
/*   Updated: 2025/11/21 10:49:18 by theo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	init_pipeline(t_pipex *pipex)
{
	int	i;

	i = 0;
	pipex->pipe_tab = malloc(sizeof(int *) * (pipex->total_cmds - 1));
	if (!pipex->pipe_tab)
		return (0);
	while (i < (pipex->total_cmds - 1))
	{
		pipex->pipe_tab[i] = malloc(sizeof(int) * 2);
		if (!pipex->pipe_tab[i])
		{
			fail_init_exit(pipex, FALSE, 0, i);
			return (0);
		}
		i ++;
	}
	i = 0;
	while (i < (pipex->total_cmds - 1))
	{
		if (pipe(pipex->pipe_tab[i]) == -1)
			return (fail_init_exit(pipex, TRUE, i, pipex->total_cmds - 1), 0);
		i ++;
	}
	return (1);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipex	pipex;
	int		return_value;

	if (argc < 5)
	{
		ft_putstr_fd("usage > ./pipex file1 cmd1 cmd2 ... file", STDERR_FILENO);
		return (1);
	}
	if (!parser(&pipex, argv, envp, argc))
		return (1);
	if (!init_pipeline(&pipex))
		return (1);
	return_value = run_pipeline(&pipex);
	clean_exit(&pipex);
	return (return_value);
}
