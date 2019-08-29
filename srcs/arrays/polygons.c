/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygons.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 21:38:16 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/29 02:12:39 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include "polygon.h"

t_polygons	*create_polygons_array(int capacity)
{
	const size_t	size = sizeof(t_polygons) + (capacity * sizeof(t_polygon));
	t_polygons	*arr;

	if (capacity <= 0)
		return (NULL);
	if (!(arr = (t_polygons*)malloc(size)))
		return (NULL);
	ft_bzero(arr, size);
	arr->capacity = capacity;
	return (arr);
}

t_polygons	*append_polygons_array(t_polygons **arr, t_polygon p)
{
	t_polygons	*new;
	size_t			old_size;

	if ((*arr)->len == (*arr)->capacity)
	{
		old_size = sizeof(t_polygons) + ((*arr)->capacity * sizeof(t_polygon));
		(*arr)->capacity *= 2;
		if (!(new = create_polygons_array((*arr)->capacity)))
			return (NULL);
		ft_memcpy(new, *arr, old_size);
		free(*arr);
		*arr = new;
	}
	(*arr)->polygons[(*arr)->len++] = p;
	return (*arr);
}

t_polygons	*splice_polygons_array(t_polygons *arr, int index, int n)
{
	if (index < 0)
		index = index + arr->len - 1;
	if (n <= 0 || index >= arr->len || index + n > arr->len)
		return (NULL);
	ft_memcpy(arr->polygons + index, arr->polygons + index + n,
		(arr->len - (index + n)) * sizeof(t_polygon));
	arr->len -= n;
	return (arr);
}

t_polygons	*copy_polygons_array(t_polygons *src, t_polygons **dst)
{
	if ((*dst)->capacity < src->capacity)
	{
		free(*dst);
		if (!(*dst = create_polygons_array(src->capacity)))
			return (NULL);
	}
	ft_memcpy(*dst, src,
		sizeof(t_polygons) + (src->capacity * sizeof(t_polygon)));
	return (*dst);
}
