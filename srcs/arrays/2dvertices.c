/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2dvertices.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 13:54:28 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/19 15:03:00 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_2dvertices	*create_2dvertices_array(int capacity)
{
	const size_t	size = sizeof(t_2dvertices) + (capacity * sizeof(t_vec2));
	t_2dvertices	*arr;

	if (capacity <= 0)
		return (NULL);
	if (!(arr = (t_2dvertices*)malloc(size)))
		return (NULL);
	ft_bzero(arr, size);
	arr->capacity = capacity;
	return (arr);
}

t_2dvertices	*append_2dvertices_array(t_2dvertices **arr, t_vec2 v)
{
	t_2dvertices	*new;
	size_t			old_size;

	if ((*arr)->len == (*arr)->capacity)
	{
		old_size = sizeof(t_2dvertices) + ((*arr)->capacity * sizeof(t_vec2));
		(*arr)->capacity *= 2;
		if (!(new = create_2dvertices_array((*arr)->capacity)))
			return (NULL);
		ft_memcpy(new, *arr, old_size);
		free(*arr);
		*arr = new;
	}
	(*arr)->vertices[(*arr)->len++] = v;
	return (*arr);
}

t_2dvertices	*splice_2dvertices_array(t_2dvertices *arr, int index, int n)
{
	if (index < 0)
		index = index + arr->len - 1;
	if (n <= 0 || index >= arr->len || index + n > arr->len)
		return (NULL);
	ft_memcpy(arr->vertices + index, arr->vertices + index + n,
		(arr->len - (index + n)) * sizeof(t_vec2));
	arr->len -= n;
	return (arr);
}

t_2dvertices	*copy_2dvertices_array(t_2dvertices *src, t_2dvertices **dst)
{
	if ((*dst)->capacity < src->capacity)
	{
		free(*dst);
		if (!(*dst = create_2dvertices_array(src->capacity)))
			return (NULL);
	}
	ft_memcpy(*dst, src,
		sizeof(t_2dvertices) + (src->capacity * sizeof(t_vec2)));
	return (*dst);
}
