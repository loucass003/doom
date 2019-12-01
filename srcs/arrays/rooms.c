/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rooms.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/22 14:29:19 by llelievr          #+#    #+#             */
/*   Updated: 2019/12/01 01:05:44 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include "editor.h"

t_rooms	*create_rooms_array(int capacity)
{
	const size_t	size = sizeof(t_rooms) + (capacity * sizeof(t_room));
	t_rooms			*arr;

	if (capacity <= 0)
		return (NULL);
	if (!(arr = (t_rooms*)malloc(size)))
		return (NULL);
	ft_bzero(arr, size);
	arr->capacity = capacity;
	return (arr);
}

t_rooms	*append_rooms_array(t_rooms **arr, t_room v)
{
	t_rooms	*new;
	size_t			old_size;

	if ((*arr)->len == (*arr)->capacity)
	{
		old_size = sizeof(t_rooms) + ((*arr)->capacity * sizeof(t_room));
		(*arr)->capacity *= 2;
		if (!(new = create_rooms_array((*arr)->capacity)))
			return (NULL);
		ft_memcpy(new, *arr, old_size);
		free(*arr);
		*arr = new;
	}
	(*arr)->values[(*arr)->len++] = v;
	return (*arr);
}

t_rooms	*splice_rooms_array(t_rooms *arr, int index, int n)
{
	if (index < 0)
		index = index + arr->len - 1;
	if (n <= 0 || index >= arr->len || index + n > arr->len)
		return (NULL);
	ft_memcpy(arr->values + index, arr->values + index + n,
		(arr->len - (index + n)) * sizeof(t_room));
	arr->len -= n;
	return (arr);
}

t_rooms	*copy_rooms_array(t_rooms *src, t_rooms **dst)
{
	if ((*dst)->capacity < src->capacity)
	{
		free(*dst);
		if (!(*dst = create_rooms_array(src->capacity)))
			return (NULL);
	}
	ft_memcpy(*dst, src,
		sizeof(t_rooms) + (src->capacity * sizeof(t_room)));
	return (*dst);
}


int		rooms_indexof(t_rooms *arr, t_room *elem)
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
