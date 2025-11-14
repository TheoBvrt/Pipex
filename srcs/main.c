/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thbouver <thbouver@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:46:18 by thbouver          #+#    #+#             */
/*   Updated: 2025/11/14 17:09:46 by thbouver         ###   ########.fr       */
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

int	init(t_pipex *pipex, char *argv[], char *envp[])
{
	char	*cmd;
	int		pipe_fds[2];
	pid_t	cmd_1;
	pid_t	cmd_2;

	pipe(pipe_fds);
	
	cmd_1 = fork();
	
	if (cmd_1 == 0)
	{
		cmd = find_path(argv[2], envp);
		if (!cmd)
		{
			ft_putstr_fd(argv[2], 2);
			ft_putstr_fd(": command not found\n", 2);
			exit (1);
		}
		close(pipe_fds[0]);
		int input = open("input_file", O_RDONLY);
		dup2(input, STDIN_FILENO);
		dup2(pipe_fds[1], STDOUT_FILENO);
		int check = execve("/usr/bin/cat", (char *[]){NULL}, envp);
		close(pipe_fds[1]);
		exit (0); 
	}
	cmd_2 = fork();
	if (cmd_2 == 0)
	{
		cmd = find_path(argv[3], envp);
		if (!cmd)
		{
			ft_putstr_fd(argv[3], 2);
			ft_putstr_fd(": command not found\n", 2);
			exit (1);
		}
		close(pipe_fds[1]);
		int output = open("output_file", O_RDWR);
		dup2(pipe_fds[0], STDIN_FILENO);
		dup2(output, STDOUT_FILENO);
		int check = execve("/usr/bin/cat", (char *[]){"-f", NULL}, envp);
		if (check < 0)
			perror("1");
		close(pipe_fds[0]);
		exit (0);
	}
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	waitpid(cmd_1, NULL, 0);
	waitpid(cmd_2, NULL, 0);
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipex	pipex;

	if (argc != 5)
	{
		ft_putstr_fd("usage > ./pipex file1 cmd1 file 2 cmd2", STDERR_FILENO);
		return (1);
	}
	if (!init(&pipex, argv, envp))
		return (1);
	return (0);
}