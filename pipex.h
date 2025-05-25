/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 14:45:52 by abostrom          #+#    #+#             */
/*   Updated: 2025/05/25 18:40:15 by abostrom         ###   ########.fr       */
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
int		exec_command(char *command, char **envp);

#endif
