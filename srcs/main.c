/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thbouver <thbouver@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:46:18 by thbouver          #+#    #+#             */
/*   Updated: 2025/11/19 17:28:14 by thbouver         ###   ########.fr       */
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
		if (access(cmd_path, R_OK) == 0)
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

char *find_path(char *cmd, char *envp[])
{
	int		y;

	y = 0;
	while (envp[y])
	{
		if (ft_strncmp(envp[y], "PATH=", 5) == 0)
			return (get_path(cmd, envp[y]));
		y ++;
	}
}

// int	init(t_pipex *pipex, char *argv[], char *envp[])
// {
// 	char	*cmd;
// 	int		pipe_fds[2];
// 	pid_t	cmd_1;
// 	pid_t	cmd_2;

// 	pipe(pipe_fds);

// 	cmd_1 = fork();

// 	if (cmd_1 == 0)
// 	{
// 		cmd = find_path(argv[2], envp);
// 		if (!cmd)
// 		{
// 			ft_putstr_fd(argv[2], 2);
// 			ft_putstr_fd(": command not found\n", 2);
// 			exit (1);
// 		}
// 		close(pipe_fds[0]);
// 		int input = open("input_file", O_RDONLY);
// 		dup2(input, STDIN_FILENO);
// 		dup2(pipe_fds[1], STDOUT_FILENO);
// 		int check = execve("/usr/bin/cat", (char *[]){NULL}, envp);
// 		close(pipe_fds[1]);
// 		exit (0);
// 	}
// 	cmd_2 = fork();
// 	if (cmd_2 == 0)
// 	{
// 		cmd = find_path(argv[3], envp);
// 		if (!cmd)
// 		{
// 			ft_putstr_fd(argv[3], 2);
// 			ft_putstr_fd(": command not found\n", 2);
// 			exit (1);
// 		}
// 		close(pipe_fds[1]);
// 		int output = open("output_file", O_RDWR);
// 		dup2(pipe_fds[0], STDIN_FILENO);
// 		dup2(output, STDOUT_FILENO);
// 		int check = execve("/usr/bin/cat", (char *[]){"-f", NULL}, envp);
// 		if (check < 0)
// 			perror("1");
// 		close(pipe_fds[0]);
// 		exit (0);
// 	}
// 	close(pipe_fds[0]);
// 	close(pipe_fds[1]);
// 	waitpid(cmd_1, NULL, 0);
// 	waitpid(cmd_2, NULL, 0);
// 	return (0);
// }

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

int	exec(t_pipex *pipex)
{
	int	current_pid;
	int	index;
	int	**pipe_tab;

	index = 0;
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
		current_pid = fork();
		if (current_pid == 0)
		{
			char *cmd = find_path(pipex->cmds[index].cmd, pipex->envp);
			if (index == 0)
			{
				pipex->in_fd = open(pipex->file_in, O_RDONLY);
				dup2(pipex->in_fd, STDIN_FILENO);
				close(pipex->in_fd);
			}
			else
				dup2(pipe_tab[index - 1][0], STDIN_FILENO);
			
			if (index == (pipex->total_cmds - 1))
			{
				pipex->out_fd = open(pipex->file_out, O_RDWR);
				dup2(pipex->out_fd, STDOUT_FILENO);
				close(pipex->out_fd);
			}
			else
				dup2(pipe_tab[index][1], STDOUT_FILENO);

			for (int i = 0; i < (pipex->total_cmds - 1); i ++)
			{
				close (pipe_tab[i][0]);
				close(pipe_tab[i][1]);
			}
			execve(cmd, (char *[]){NULL}, pipex->envp);
		}
		index ++;
	}

	index = 0;
	while (index < (pipex->total_cmds - 1))
	{
		close(pipe_tab[index][0]);
		close(pipe_tab[index][1]);
		index ++;
	}
	waitpid(current_pid, NULL, 0);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipex	pipex;

	if (argc < 5)
	{
		ft_putstr_fd("usage > ./pipex file1 cmd1 cm2 ... file", STDERR_FILENO);
		return (1);
	}
	if (!parser(&pipex, argv, envp, argc))
		return (1);
	exec(&pipex);
	return (0);
}
