/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:46:34 by abostrom          #+#    #+#             */
/*   Updated: 2025/05/25 14:53:15 by abostrom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "pipex.h"

pid_t	create_child(int in, int out, int unused)
{
	const pid_t	pid = fork();

	if (!pid && (dup2(in, 0) == -1 || dup2(out, 1) == -1 || close(unused) != 0))
	{
		perror("pipex");
		return (-1);
	}
	return (pid);
}

static int	wait_for_children(pid_t pids[2])
{
	int	s[2];

	if (waitpid(pids[0], &s[0], 0) <= 0 || waitpid(pids[1], &s[1], 0) <= 0)
	{
		perror("pipex");
		return (-1);
	}
	return (s[1]);
}

static void	open_pipe(int pipes[2])
{
	if (pipe(pipes) == -1)
	{
		perror("pipex");
		exit(1);
	}
}

static int	open_file(const char *filename, int flags)
{
	const int	fd = open(filename, flags, 0644);

	if (fd == -1)
		perror("pipex");
	return (fd);
}

int	main(int argc, char **argv, char **envp)
{
	int		pipes[2];
	int		files[2];
	pid_t	pids[2];

	if (argc != 5)
		return (1);
	open_pipe(pipes);
	files[0] = open_file(argv[1], O_RDONLY);
	files[1] = open_file(argv[4], O_WRONLY | O_CREAT | O_TRUNC);
	pids[0] = create_child(files[0], pipes[1], pipes[0]);
	if (pids[0] == 0)
		exec_command(argv[2], envp);
	pids[1] = create_child(pipes[0], files[1], pipes[1]);
	if (pids[1] == 0)
		exec_command(argv[3], envp);
	close(pipes[0]);
	close(pipes[1]);
	close(files[0]);
	close(files[1]);
	return (wait_for_children(pids));
}
