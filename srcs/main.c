/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thbouver <thbouver@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:46:18 by thbouver          #+#    #+#             */
/*   Updated: 2025/11/20 17:33:50 by thbouver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_path(char *cmd, char *path)
{
	char	**tmp;
	char	*cmd_path;
	int		index;

	index = 0;
	cmd_path = NULL;
	tmp = ft_split(path + 5, ':');
	if (!tmp)
		return (NULL);
	while(tmp[index])
	{
		cmd_path = ft_strcat(cmd_path, tmp[index]);
		cmd_path = ft_strcat(cmd_path, "/");
		cmd_path = ft_strcat(cmd_path, cmd);
		if (access(cmd_path, F_OK) == 0)
		{
			free_tab(tmp);
			return (cmd_path);
		}
		free (cmd_path);
		cmd_path = NULL;
		index ++;
	}
	free_tab(tmp);
	return (cmd_path);
}

char *find_path(char *cmd, char *envp[], int *status)
{
	char	*path;
	int		y;

	y = 0;
	if (access(cmd, F_OK) == 0)
	{
		if (access(cmd, X_OK) == -1)
		{
			*status = -1;
			return (NULL);
		}
		return (cmd);
	}
	while (envp[y])
	{
		if (ft_strncmp(envp[y], "PATH=", 5) == 0)
		{
			path = get_path(cmd, envp[y]);
			if (path)
			{
				if (access(path, X_OK) == -1)
				{
					free (path);
					*status = -1;
					return (NULL);
				}
			}
			*status = -2;
			return (path);
		}
		y ++;
	}
}

void	debug(t_pipex pipex)
{
	int	index;

	index = 0;
	printf("Infile : %s\n", pipex.file_in);
	printf("Outfile : %s\n", pipex.file_out);
	while (index < pipex.total_cmds)
	{
		ft_printf("_____________\n");
		ft_printf("| Commands : \n");
		ft_printf("| 	%s\n", pipex.cmds[index].cmd);
		ft_printf("| Arguments : \n");
		while (*pipex.cmds[index].args)
		{
			ft_printf("| 	%s\n", *pipex.cmds[index].args);
			pipex.cmds[index].args ++;
		}
		ft_printf("|\n");
		ft_printf("_____________\n");
		index ++;
	}
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

int	exec(t_pipex *pipex)
{
	int	index;
	int	**pipe_tab;
	int	status;

	index = 0;
	status = -2;
	pipe_tab = malloc(sizeof(int *) * (pipex->total_cmds - 1));
	if (!pipe_tab)
		return (0);
	while (index < (pipex->total_cmds - 1))
	{
		pipe_tab[index] = malloc(sizeof(int) * 2);
		if (!pipe_tab[index])
			return (0);
		index ++;
	}
	index = 0;
	while (index < (pipex->total_cmds - 1))
		pipe(pipe_tab[index ++]);

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
					close_all(pipex, pipe_tab);
					exit(126);
				}
				ft_putstr_fd(pipex->cmds[index].cmd, 2);
				ft_putstr_fd(": command not found\n", 2);
				close_all(pipex, pipe_tab);
				exit (127);
			}
			if (index == 0)
			{
				pipex->in_fd = open(pipex->file_in, O_RDONLY);
				if (pipex->in_fd == -1)
				{
					perror("pipex");
					close_all(pipex, pipe_tab);
					exit (1);
				}
				dup2(pipex->in_fd, STDIN_FILENO);
				close(pipex->in_fd);
			}
			else
				dup2(pipe_tab[index - 1][0], STDIN_FILENO);
			if (index == (pipex->total_cmds - 1))
			{
				pipex->out_fd = open(pipex->file_out, O_CREAT | O_WRONLY | O_TRUNC, 0644);
				if (pipex->out_fd == -1)
				{
					perror("pipex");
					close_all(pipex, pipe_tab);
					exit(1);
				}
				dup2(pipex->out_fd, STDOUT_FILENO);
				close(pipex->out_fd);
			}
			else
				dup2(pipe_tab[index][1], STDOUT_FILENO);
			close_all(pipex, pipe_tab);
			if (execve(pipex->cmd, pipex->cmds[index].args, pipex->envp) == -1)
			{
				perror("pipex");
				exit(1);
			}
			exit (0);
		}
		index ++;
	}

	close_all(pipex, pipe_tab);
	index  = 0;
	while (index < pipex->total_cmds)
		waitpid(pipex->cmds[index ++].pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (status);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipex	pipex;

	if (argc < 5)
	{
		ft_putstr_fd("usage > ./pipex file1 cmd1 cmd2 ... file", STDERR_FILENO);
		return (1);
	}
	int	check;
	if (!parser(&pipex, argv, envp, argc))
		return (1);
	return (exec(&pipex));
}
