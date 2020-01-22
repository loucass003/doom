/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/22 14:02:50 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/22 14:37:00 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include "editor.h"

t_objects	*create_objects_array(int capacity)
{
	const size_t	size = sizeof(t_objects) + (capacity * sizeof(t_object));
	t_objects		*arr;

	if (capacity <= 0)
		return (NULL);
	if (!(arr = (t_objects*)malloc(size)))
		return (NULL);
	ft_bzero(arr, size);
	arr->capacity = capacity;
	return (arr);
}

t_objects	*append_objects_array(t_objects **arr, t_object v)
{
	t_objects		*new;
	size_t			old_size;

	if ((*arr)->len == (*arr)->capacity)
	{
		old_size = sizeof(t_objects) + ((*arr)->capacity * sizeof(t_object));
		(*arr)->capacity *= 2;
		if (!(new = create_objects_array((*arr)->capacity)))
			return (NULL);
		ft_memcpy(new, *arr, old_size);
		free(*arr);
		*arr = new;
	}
	(*arr)->values[(*arr)->len++] = v;
	return (*arr);
}

t_objects	*splice_objects_array(t_objects *arr, int index, int n)
{
	if (index < 0)
		index = index + arr->len - 1;
	if (n <= 0 || index >= arr->len || index + n > arr->len)
		return (NULL);
	ft_memcpy(arr->values + index, arr->values + index + n,
		(arr->len - (index + n)) * sizeof(t_object));
	arr->len -= n;
	return (arr);
}

t_objects	*copy_objects_array(t_objects *src, t_objects **dst)
{
	if ((*dst)->capacity < src->capacity)
	{
		free(*dst);
		if (!(*dst = create_objects_array(src->capacity)))
			return (NULL);
	}
	ft_memcpy(*dst, src,
		sizeof(t_objects) + (src->capacity * sizeof(t_object)));
	return (*dst);
}

int			objects_indexof(t_objects *arr, t_object *elem)
{
	int	i;

	i = -1;
	while (++i < arr->len)
	{
		if (arr->values + i == elem)
			return (i);
	}
	return (-1);
}
