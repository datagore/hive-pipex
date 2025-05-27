/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 14:48:43 by abostrom          #+#    #+#             */
/*   Updated: 2025/05/28 00:01:15 by abostrom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

char	**make_argv_array(char *command)
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

char	*find_path_in_envp(char **envp)
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

char	*find_command_in_path(char **argv, char *path)
{
	char	*end;
	char	*cmd_path;
	size_t	dir_length;

	if (argv == NULL || argv[0] == NULL || path == NULL)
		return (NULL);
	while (*path != '\0')
	{
		end = path;
		while (*end != ':' && *end != '\0')
			end++;
		dir_length = end - path;
		cmd_path = malloc(ft_strlen(argv[0]) + dir_length + 2);
		if (cmd_path == NULL)
			return (NULL);
		ft_memcpy(cmd_path, path, dir_length);
		cmd_path[dir_length] = '/';
		ft_memcpy(cmd_path + dir_length + 1, argv[0], ft_strlen(argv[0]) + 1);
		if (access(cmd_path, F_OK | X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		path = end + (*end == ':');
	}
	return (NULL);
}
