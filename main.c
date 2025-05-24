/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:46:34 by abostrom          #+#    #+#             */
/*   Updated: 2025/05/25 00:05:44 by abostrom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
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

char	*find_path_in_env(char **envp)
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

	while (path != NULL && *path != '\0')
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

void	error(const char *message)
{
	write(2, message, ft_strlen(message));
}

void	exec_command(char *command, char **envp)
{
	char *const		path = find_path_in_env(envp);
	char **const	argv = make_argv_array(command);
	char *const		file = find_command_in_path(argv[0], path);

	execve(file, argv, envp);
	perror("pipex");
	free(file);
	free(argv);
}

pid_t	fork_and_redirect(int in, int out, int unused)
{
	const pid_t	pid = fork();

	if (!pid && (dup2(in, 0) == -1 || dup2(out, 1) == -1 || close(unused) != 0))
		return (-1);
	return (pid);
}

int	main(int argc, char **argv, char **envp)
{
	int		pipes[2];
	int		files[2];
	pid_t	pids[2];

	if (argc != 5)
		return (1);
	pipe(pipes);
	files[0] = open(argv[1], O_RDONLY);
	files[1] = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	pids[0] = fork_and_redirect(files[0], pipes[1], pipes[0]);
	if (pids[0] == 0)
		exec_command(argv[2], envp);
	pids[1] = fork_and_redirect(pipes[0], files[1], pipes[1]);
	if (pids[1] == 0)
		exec_command(argv[3], envp);
	close(pipes[0]);
	close(pipes[1]);
	close(files[0]);
	close(files[1]);
	waitpid(pids[0], NULL, 0);
	waitpid(pids[1], NULL, 0);
}
