/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:46:34 by abostrom          #+#    #+#             */
/*   Updated: 2025/05/28 00:10:23 by abostrom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "pipex.h"

static int	error(int status, const char *message)
{
	write(2, "pipex: ", 7);
	perror(message);
	return (status);
}

static int	open_file(const char *filename, int fds[2], int index)
{
	const int	flags[2] = {O_RDONLY, O_WRONLY | O_CREAT | O_TRUNC};
	const int	file = open(filename, flags[index], 0644);

	if (file != -1)
		return (file);
	close(fds[!index]);
	exit(error(1, filename));
}

static void	run_command(char *command, char **envp, int fds[2])
{
	char *const		path = find_path_in_envp(envp);
	char **const	argv = make_argv_array(command);
	char *const		file = find_command_in_path(argv, path);

	if (file == NULL)
	{
		free(argv);
		close(fds[0]);
		close(fds[1]);
		write(2, "pipex: ", 7);
		write(2, command, ft_strlen(command));
		write(2, ": command not found\n", 20);
		exit(127);
	}
	dup2(fds[0], 0);
	dup2(fds[1], 1);
	close(fds[0]);
	close(fds[1]);
	execve(file, argv, envp);
	free(file);
	free(argv);
	exit(error(1, "execve"));
}

static pid_t	run_subprocess(char **argv, char **envp, int fds[2], int index)
{
	const pid_t	pid = fork();

	if (pid < 0)
		return (error(1, "fork"));
	if (pid > 0)
		return (pid);
	close(fds[index]);
	fds[index] = open_file(argv[1 + index * 3], fds, index);
	run_command(argv[2 + index], envp, fds);
	return (pid);
}

int	main(int argc, char **argv, char **envp)
{
	int		status;
	int		pipes[2];
	pid_t	cmd1;
	pid_t	cmd2;

	if (argc != 5)
		return (1);
	if (pipe(pipes) == -1)
		return (error(1, "pipe"));
	cmd1 = run_subprocess(argv, envp, pipes, 0);
	cmd2 = run_subprocess(argv, envp, pipes, 1);
	close(pipes[0]);
	close(pipes[1]);
	waitpid(cmd1, &status, 0);
	waitpid(cmd2, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
