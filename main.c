/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:46:34 by abostrom          #+#    #+#             */
/*   Updated: 2025/05/23 17:33:54 by abostrom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

	argc = 1 + split_params(command, NULL);
	argv = malloc(argc * sizeof(char *));
	if (argv != NULL)
	{
		split_params(command, argv);
		argv[argc] = NULL;
	}
	return (argv);
}

int	ft_strncmp(const char *a, const char *b, size_t length)
{
	size_t	i;

	i = 0;
	while ((a[i] != '\0' || b[i] != '\0') && i < length)
	{
		if (a[i] != b[i])
			return (a[i] - b[i]);
		i++;
	}
	return (0);
}

size_t	ft_strlen(const char *str)
{
	size_t	length;

	length = 0;
	while (str[length] != '\0')
		length++;
	return (length);
}

char	*find_path_in_envp(char **envp)
{
	char	*equals;

	while (*envp != NULL)
	{
		equals = *envp;
		while (*equals != '=')
			equals++;
		if (ft_strncmp(*envp, "PATH", equals - *envp) == 0)
			return (equals + 1);
		envp++;
	}
	return (NULL);
}

char	*find_command_in_path(const char *cmd_name, char *path)
{
	const size_t	cmd_length = ft_strlen(cmd_name);
	char			*cmd_path;
	char			*end;

	while (*path != '\0')
	{
		end = path;
		while (*end != ':' && *end != '\0')
			end++;
		cmd_path = malloc(cmd_length + (end - path) + 2);
		if (cmd_path == NULL)
			return (NULL);
		__builtin_memcpy(cmd_path, path, end - path);
		cmd_path[end - path] = '/';
		__builtin_memcpy(cmd_path + (end - path) + 1, cmd_name, cmd_length + 1);
		if (access(cmd_path, F_OK | X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		path = end + (*end == ':');
	}
	return (NULL);
}

int	main(int argc, char **argv, char **envp)
{
	(void) argc;
	(void) argv;
	char	*path = find_path_in_envp(envp);
	char	*cmd = find_command_in_path(argv[1], path);
	__builtin_printf("%s\n", cmd);
	free(cmd);
}
