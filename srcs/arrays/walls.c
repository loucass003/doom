/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/22 14:02:50 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/08 20:10:17 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include "editor.h"

t_walls	*create_walls_array(int capacity)
{
	const size_t	size = sizeof(t_walls) + (capacity * sizeof(t_wall));
	t_walls			*arr;

	if (capacity <= 0)
		return (NULL);
	if (!(arr = (t_walls*)malloc(size)))
		return (NULL);
	ft_bzero(arr, size);
	arr->capacity = capacity;
	return (arr);
}

t_walls	*append_walls_array(t_walls **arr, t_wall v)
{
	t_walls			*new;
	size_t			old_size;

	if ((*arr)->len == (*arr)->capacity)
	{
		old_size = sizeof(t_walls) + ((*arr)->capacity * sizeof(t_wall));
		(*arr)->capacity *= 2;
		if (!(new = create_walls_array((*arr)->capacity)))
			return (NULL);
		ft_memcpy(new, *arr, old_size);
		free(*arr);
		*arr = new;
	}
	(*arr)->values[(*arr)->len++] = v;
	return (*arr);
}

t_walls	*splice_walls_array(t_walls *arr, int index, int n)
{
	int		i;

	if (index < 0)
		index = index + arr->len - 1;
	if (n <= 0 || index >= arr->len || index + n > arr->len)
		return (NULL);
	i = -1;
	while (++i < n)
		free_wall(&arr->values[index + i]);
	ft_memcpy(arr->values + index, arr->values + index + n,
		(arr->len - (index + n)) * sizeof(t_wall));
	arr->len -= n;
	return (arr);
}

t_walls	*copy_walls_array(t_walls *src, t_walls **dst)
{
	if ((*dst)->capacity < src->capacity)
	{
		free(*dst);
		if (!(*dst = create_walls_array(src->capacity)))
			return (NULL);
	}
	ft_memcpy(*dst, src,
		sizeof(t_walls) + (src->capacity * sizeof(t_wall)));
	return (*dst);
}

int		walls_indexof(t_walls *arr, t_wall *elem)
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
