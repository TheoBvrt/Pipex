/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thbouver <thbouver@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:46:18 by thbouver          #+#    #+#             */
/*   Updated: 2025/11/13 17:59:48 by thbouver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_path(char *cmd, char *path)
{
	char	**tmp;
	char	*new;
	int		index;

	index = 0;
	tmp = ft_split(path + 5, ':');
	if (!tmp)
		return (NULL);
	while(tmp[index])
	{
		// new = ft_strjoin(tmp[index], )
		index ++;
	}
}

char **find_path(char *cmd, char *envp[])
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
	int		pipe_fds[2];
	pid_t	cmd_1;
	pid_t	cmd_2;

	pipe(pipe_fds);
	
	cmd_1 = fork();
	if (cmd_1 > 0)
		cmd_2 = fork();
	
	if (cmd_1 == 0)
	{
		close(pipe_fds[0]);
		int input = open("input_file", O_RDONLY);
		dup2(input, STDIN_FILENO);
		dup2(pipe_fds[1], STDOUT_FILENO);
		execve("/usr/bin/cat", (char *[]){"usr/bin/cat", NULL}, envp);
	}
	if (cmd_2 == 0)
	{	
		close(pipe_fds[1]);
		int output = open("output_file", O_RDWR);
		dup2(pipe_fds[0], STDIN_FILENO);
		dup2(output, STDOUT_FILENO);
		int check = execve("/usr/bin/cat", (char *[]){"usr/bin/cat", NULL}, envp);
		printf("%d", check);
	}
	if (cmd_1 > 0 && cmd_2 > 0)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		waitpid(cmd_1, NULL, 0);
		waitpid(cmd_2, NULL, 0);
	}
	
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