/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 14:48:43 by abostrom          #+#    #+#             */
/*   Updated: 2025/05/25 18:40:06 by abostrom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pipex.h"

static int	split_params(char *string, char **params)
{
	int		count;
	char	*start;

	count = 0;
	while (*string != '\0')
	{
		while (*string == ' ')
			string++;
		start = string;
		while (*string != ' ' && *string != '\0')
			string++;
		if (start < string)
		{
			if (params != NULL)
			{
				params[count] = start;
				if (*string != '\0')
					*string++ = '\0';
			}
			count++;
		}
	}
	return (count);
}

static char	**make_argv_array(char *command)
{
	char	**argv;
	int		argc;

	argc = split_params(command, NULL);
	argv = malloc((argc + 1) * sizeof(char *));
	if (argv != NULL)
	{
		split_params(command, argv);
		argv[argc] = NULL;
	}
	return (argv);
}

static char	*find_path_in_env(char **envp)
{
	char	*equals;

	while (envp != NULL && *envp != NULL)
	{
		equals = ft_strchr(*envp, '=');
		if (ft_strncmp(*envp, "PATH", equals - *envp) == 0)
			return (equals + 1);
		envp++;
	}
	return (NULL);
}

static char	*find_command_in_path(const char *cmd_name, const char *path)
{
	const size_t	cmd_length = ft_strlen(cmd_name);
	const char		*end;
	char			*cmd_path;
	size_t			dir_length;

	while (cmd_name != NULL && path != NULL && *path != '\0')
	{
		end = path;
		while (*end != ':' && *end != '\0')
			end++;
		dir_length = end - path;
		cmd_path = malloc(cmd_length + dir_length + 2);
		if (cmd_path == NULL)
			return (NULL);
		ft_memcpy(cmd_path, path, dir_length);
		cmd_path[dir_length] = '/';
		ft_memcpy(cmd_path + dir_length + 1, cmd_name, cmd_length + 1);
		if (access(cmd_path, F_OK | X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		path = end + (*end == ':');
	}
	return (NULL);
}

int	exec_command(char *command, char **envp)
{
	char *const		path = find_path_in_env(envp);
	char **const	argv = make_argv_array(command);
	char *const		file = find_command_in_path(argv[0], path);

	if (file == NULL)
	{
		ft_putstr_fd(2, "pipex: command not found: ");
		ft_putstr_fd(2, command);
		ft_putstr_fd(2, "\n");
		free(file);
		free(argv);
		return (127);
	}
	else
	{
		execve(file, argv, envp);
		perror("pipex");
		free(file);
		free(argv);
		return (EXIT_FAILURE);
	}
}
