/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abostrom <abostrom@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 14:44:35 by abostrom          #+#    #+#             */
/*   Updated: 2025/05/25 14:45:25 by abostrom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "pipex.h"

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

void	*ft_memcpy(void *dst, const void *src, size_t size)
{
	char *const	dst_bytes = (char *) dst;
	char *const	src_bytes = (char *) src;

	while (size-- != 0)
		dst_bytes[size] = src_bytes[size];
	return (dst);
}

void	ft_putstr_fd(int fd, const char *string)
{
	write(fd, string, ft_strlen(string));
}
