/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thbouver <thbouver@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 16:23:51 by thbouver          #+#    #+#             */
/*   Updated: 2025/11/21 16:04:45 by thbouver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*ft_strcat(char *dest, char *src)
{
	char	*tmp;

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

static char	*get_path(char *cmd, char *path)
{
	char	**tmp;
	char	*cmd_path;
	int		index;

	index = 0;
	cmd_path = NULL;
	tmp = ft_split(path + 5, ':');
	if (!tmp)
		return (NULL);
	while (tmp[index])
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

static int	parse_command(t_command *cmd, char *str)
{
	char	**token;
	int		index;

	index = 0;
	token = ft_split(str, ' ');
	if (!token)
		return (0);
	cmd->args = ft_calloc(count_tab(token) + 1, sizeof(char *));
	if (!cmd->args)
		return (free_tab(cmd->args), 0);
	cmd->cmd = token[index];
	while (token[index])
	{
		cmd->args[index] = token[index];
		index ++;
	}
	return (free(token), 1);
}

char	*find_path(char *cmd, char *envp[], int *status)
{
	char	*path;
	int		y;

	y = 0;
	if (access(cmd, F_OK) == 0)
	{
		if (is_dir(cmd) == 1 && cmd[0] != '.' && cmd[1] != '/')
			return (*status = 0, NULL);
		if (access(cmd, X_OK) == -1)
			return (*status = -1, NULL);
		return (cmd);
	}
	while (envp[y])
	{
		if (ft_strncmp(envp[y], "PATH=", 5) == 0)
		{
			path = get_path(cmd, envp[y]);
			if (path)
				if (access(path, X_OK) == -1)
					return (free(path), *status = -1, NULL);
			return (path);
		}
		y ++;
	}
	return (NULL);
}

int	parser(t_pipex *pipex, char **argv, char **envp, int argc)
{
	int	index;

	index = 0;
	pipex->cmds = malloc(sizeof(t_command) * (argc - 3));
	if (!pipex->cmds)
		return (1);
	pipex->file_in = argv[1];
	pipex->file_out = argv[argc - 1];
	pipex->total_cmds = (argc - 3);
	pipex->envp = envp;
	while (index < pipex->total_cmds)
	{
		if (!parse_command(&pipex->cmds[index], argv[index + 2]))
			return (free(pipex->cmds), 0);
		index ++;
	}
	return (1);
}
