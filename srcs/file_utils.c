/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/28 01:48:23 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/16 15:44:34 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <libft.h>

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
