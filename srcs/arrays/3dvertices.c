/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3dvertices.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 13:54:28 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/29 19:50:11 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include "arrays.h"

t_3dvertices	*create_3dvertices_array(int capacity)
{
	const size_t	size = sizeof(t_3dvertices) + (capacity * sizeof(t_vec3));
	t_3dvertices	*arr;

	if (capacity <= 0)
		return (NULL);
	if (!(arr = (t_3dvertices*)malloc(size)))
		return (NULL);
	ft_bzero(arr, size);
	arr->capacity = capacity;
	return (arr);
}

t_3dvertices	*append_3dvertices_array(t_3dvertices **arr, t_vec3 v)
{
	t_3dvertices	*new;
	size_t			old_size;

	if ((*arr)->len == (*arr)->capacity)
	{
		old_size = sizeof(t_3dvertices) + ((*arr)->capacity * sizeof(t_vec3));
		(*arr)->capacity *= 2;
		if (!(new = create_3dvertices_array((*arr)->capacity)))
			return (NULL);
		ft_memcpy(new, *arr, old_size);
		free(*arr);
		*arr = new;
	}
	(*arr)->vertices[(*arr)->len++] = v;
	return (*arr);
}

t_3dvertices	*splice_3dvertices_array(t_3dvertices *arr, int index, int n)
{
	if (index < 0)
		index = index + arr->len - 1;
	if (n <= 0 || index > arr->len || index + n > arr->len)
		return (NULL);
	ft_memcpy(arr->vertices + index, arr->vertices + index + n,
		(arr->len - (index + n)) * sizeof(t_vec3));
	arr->len -= n;
	return (arr);
}

t_3dvertices	*copy_3dvertices_array(t_3dvertices *src, t_3dvertices **dst)
{
	if ((*dst)->capacity < src->capacity)
	{
		free(*dst);
		if (!(*dst = create_3dvertices_array(src->capacity)))
			return (NULL);
	}
	ft_memcpy(*dst, src,
		sizeof(t_3dvertices) + (src->capacity * sizeof(t_vec3)));
	return (*dst);
}
