/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theo <theo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 16:23:51 by thbouver          #+#    #+#             */
/*   Updated: 2025/11/20 23:35:01 by theo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	cmd->cmd =token[index];
	while (token[index])
	{
		cmd->args[index] = token[index];
		index ++;
	}
	return (free(token), 1);
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
