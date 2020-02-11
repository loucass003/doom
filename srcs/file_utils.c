/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/28 01:48:23 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/11 12:42:40 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <libft.h>
#include "io.h"
#include <fcntl.h>
#include <unistd.h>

char	*path_join(char *dir, char *file)
{
	size_t		len;
	size_t		dir_len;
	t_bool		need_slash;
	char		*path;

	if (!dir || !file)
		return (NULL);
	dir_len = ft_strlen(dir);
	need_slash = dir[dir_len - 1] != '/';
	len = dir_len + ft_strlen(file) + need_slash + 1;
	if (!(path = (char *)malloc(len * sizeof(char))))
		return (NULL);
	ft_strcpy(path, dir);
	if (need_slash)
		path[dir_len] = '/';
	ft_strcpy(path + dir_len + need_slash, file);
	path[len - 1] = '\0';
	return (path);
}

ssize_t		get_file_size(int fd)
{
	size_t len = lseek(fd, 0, SEEK_END);
	if (lseek(fd, 0, SEEK_SET) == -1)
		return (-1);
	return (len);
}

char			*get_file_content(char *file, int *content_len)
{
	int		fd;
	char	*content;
	char	buf[READ_BUFFER + 1];
	int		rd;

	if ((fd = open(file, O_RDONLY)) == -1)
		return (NULL);
	*content_len = 0;
	content = NULL;
	while ((rd = read(fd, &buf, READ_BUFFER)) > 0)
	{
		buf[rd] = 0;
		content = ft_realloc(content, *content_len, *content_len + rd);
		ft_memcpy(content + *content_len, buf, rd);
		*content_len += rd;
	}
	if (rd != -1)
		return (content);
	if (content)
		free(content);
	return (NULL);
}