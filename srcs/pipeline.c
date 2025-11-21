/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thbouver <thbouver@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 09:17:34 by theo              #+#    #+#             */
/*   Updated: 2025/11/21 14:58:19 by thbouver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	error_handling(t_pipex *pipex, int status, int current_index)
{
	if (status == -1)
	{
		ft_putstr_fd(pipex->cmds[current_index].cmd, 2);
		ft_putstr_fd(": permission denied\n", 2);
		return (clean_child(pipex, 126));
	}
	ft_putstr_fd(pipex->cmds[current_index].cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
	return (clean_child(pipex, 127));
}

static void	setup_input_redirection(t_pipex *pipex, int index)
{
	if (index == 0)
	{
		pipex->in_fd = open(pipex->file_in, O_RDONLY);
		if (pipex->in_fd == -1)
		{
			perror("pipex");
			exit(clean_child(pipex, 1));
		}
		dup2(pipex->in_fd, STDIN_FILENO);
		close(pipex->in_fd);
	}
	else
		dup2(pipex->pipe_tab[index - 1][0], STDIN_FILENO);
}

static void	setup_output_redirection(t_pipex *pipex, int index)
{
	if (index == (pipex->total_cmds - 1))
	{
		pipex->out_fd = open(pipex->file_out, O_CREAT
				| O_WRONLY | O_TRUNC, 0644);
		if (pipex->out_fd == -1)
		{
			perror("pipex");
			exit(clean_child(pipex, 1));
		}
		dup2(pipex->out_fd, STDOUT_FILENO);
		close(pipex->out_fd);
	}
	else
		dup2(pipex->pipe_tab[index][1], STDOUT_FILENO);
}

static void	run_child_process(t_pipex *pipex, int index)
{
	int	status;

	status = 0;
	pipex->cmd = find_path(pipex->cmds[index].cmd, pipex->envp, &status);
	if (!pipex->cmd)
		exit(error_handling(pipex, status, index));
	setup_input_redirection(pipex, index);
	setup_output_redirection(pipex, index);
	close_all(pipex->pipe_tab, pipex->total_cmds - 1);
	if (execve(pipex->cmd, pipex->cmds[index].args, pipex->envp) == -1)
	{
		perror("pipex");
		if (errno != 13)
			free(pipex->cmd);
		clean_exit(pipex);
		exit(1);
	}
}

int	run_pipeline(t_pipex *pipex)
{
	int	index;
	int	status;

	status = 0;
	index = 0;
	while (index < pipex->total_cmds)
	{
		pipex->cmds[index].pid = fork();
		if (pipex->cmds[index].pid == 0)
			run_child_process(pipex, index);
		index++;
	}
	close_all(pipex->pipe_tab, pipex->total_cmds - 1);
	index = 0;
	while (index < pipex->total_cmds)
		waitpid(pipex->cmds[index++].pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (status);
}
