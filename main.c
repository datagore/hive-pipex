/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:46:34 by abostrom          #+#    #+#             */
/*   Updated: 2025/05/23 23:44:06 by abostrom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

int	ft_strncmp(const char *a, const char *b, size_t length)
{
	while (*a && length--)
		if (*a++ != *b++)
			return (a[-1] - b[-1]);
	return (0);
}

char	*ft_strchr(const char *str, int chr)
{
	while (*str != '\0' || chr == '\0')
		if (*str++ == chr)
			return ((char *) str - 1);
	return (NULL);
}

size_t	ft_strlen(const char *string)
{
	return (ft_strchr(string, '\0') - string);
}

const char	*find_path_in_env(char **envp)
{
	const char	*equals;

	while (*envp != NULL)
	{
		equals = ft_strchr(*envp, '=');
		if (ft_strncmp(*envp, "PATH", equals - *envp) == 0)
			return (equals + 1);
		envp++;
	}
	return (NULL);
}

void	*ft_memcpy(void *dst, const void *src, size_t size)
{
	char *const	dst_bytes = (char *) dst;
	char *const	src_bytes = (char *) src;

	while (size-- != 0)
		dst_bytes[size] = src_bytes[size];
	return (dst);
}

char	*find_command_in_path(const char *cmd_name, const char *path)
{
	const size_t	cmd_length = ft_strlen(cmd_name);
	const char		*end;
	char			*cmd_path;
	size_t			dir_length;

	while (*path != '\0')
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

void	create_child_process(char *command, char **envp)
{
	const char *const	path = find_path_in_env(envp);
	char **const		argv = make_argv_array(command);
	char *const			pathname = find_command_in_path(argv[0], path);

	execve(pathname, argv, envp);
	perror("pipex");
	free(pathname);
	free(argv);
}

int	main(int argc, char **argv, char **envp)
{
	if (argc != 2)
		return (1);
	create_child_process(argv[1], envp);
}
