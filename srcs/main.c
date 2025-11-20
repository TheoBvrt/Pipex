/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theo <theo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:46:18 by thbouver          #+#    #+#             */
/*   Updated: 2025/11/21 00:31:45 by theo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	clean_child(t_pipex *pipex, int exit_value)
{
	close_all(pipex, pipex->pipe_tab);
	free(pipex->cmd);
	clean_exit(pipex);
	return (exit_value);
}

int	init_pipeline(t_pipex *pipex)
{
	int	index;

	index = 0;
	pipex->pipe_tab = malloc(sizeof(int *) * (pipex->total_cmds - 1));
	if (!pipex->pipe_tab)
		return (0);
	while (index < (pipex->total_cmds - 1))
	{
		pipex->pipe_tab[index] = malloc(sizeof(int) * 2);
		if (!pipex->pipe_tab[index])
			return (0);
		index ++;
	}
	index = 0;
	while (index < (pipex->total_cmds - 1))
		pipe(pipex->pipe_tab[index ++]);
	return (1);
}

int	exec(t_pipex *pipex)
{
	int	index;
	int	status;

	status = -2;
	index = 0;
	while (index < pipex->total_cmds)
	{
		pipex->cmds[index].pid = fork();
		if (pipex->cmds[index].pid == 0)
		{
			pipex->cmd = find_path(pipex->cmds[index].cmd, pipex->envp, &status);
			if (!pipex->cmd)
			{
				if (status == -1)
				{
					ft_putstr_fd(pipex->cmds[index].cmd, 2);
					ft_putstr_fd(": permission denied\n", 2);
					exit(clean_child(pipex, 126));
				}
				ft_putstr_fd(pipex->cmds[index].cmd, 2);
				ft_putstr_fd(": command not found\n", 2);
				exit (clean_child(pipex, 127));
			}
			if (index == 0)
			{
				pipex->in_fd = open(pipex->file_in, O_RDONLY);
				if (pipex->in_fd == -1)
				{
					perror("pipex");
					exit (clean_child(pipex, 1));
				}
				dup2(pipex->in_fd, STDIN_FILENO);
				close(pipex->in_fd);
			}
			else
				dup2(pipex->pipe_tab[index - 1][0], STDIN_FILENO);
			if (index == (pipex->total_cmds - 1))
			{
				pipex->out_fd = open(pipex->file_out, O_CREAT | O_WRONLY | O_TRUNC, 0644);
				if (pipex->out_fd == -1)
				{
					perror("pipex");
					exit (clean_child(pipex, 1));
				}
				dup2(pipex->out_fd, STDOUT_FILENO);
				close(pipex->out_fd);
			}
			else
				dup2(pipex->pipe_tab[index][1], STDOUT_FILENO);
			close_all(pipex, pipex->pipe_tab);
			if (execve(pipex->cmd, pipex->cmds[index].args, pipex->envp) == -1)
			{
				perror("pipex");
				free(pipex->cmd);
				clean_exit(pipex);
				exit(1);
			}
		}
		index ++;
	}

	close_all(pipex, pipex->pipe_tab);
	index = 0;
	while (index < pipex->total_cmds)
		waitpid(pipex->cmds[index ++].pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (status);
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
	return_value = (exec(&pipex));
	clean_exit(&pipex);
	return (return_value);
}
