/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 14:45:52 by abostrom          #+#    #+#             */
/*   Updated: 2025/05/27 23:58:47 by abostrom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdint.h>

int		ft_strncmp(const char *a, const char *b, size_t length);
char	*ft_strchr(const char *str, int chr);
size_t	ft_strlen(const char *string);
void	*ft_memcpy(void *dst, const void *src, size_t size);
void	ft_putstr_fd(int fd, const char *string);
char	**make_argv_array(char *command);
char	*find_path_in_envp(char **envp);
char	*find_command_in_path(char **argv, char *path);

#endif
