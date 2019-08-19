/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4dvertices.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 13:54:28 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/19 15:02:50 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_4dvertices	*create_4dvertices_array(int capacity)
{
	const size_t	size = sizeof(t_4dvertices) + (capacity * sizeof(t_vec4));
	t_4dvertices	*arr;

	if (capacity <= 0)
		return (NULL);
	if (!(arr = (t_4dvertices*)malloc(size)))
		return (NULL);
	ft_bzero(arr, size);
	arr->capacity = capacity;
	return (arr);
}

t_4dvertices	*append_4dvertices_array(t_4dvertices **arr, t_vec4 v)
{
	t_4dvertices	*new;
	size_t			old_size;

	if ((*arr)->len == (*arr)->capacity)
	{
		old_size = sizeof(t_4dvertices) + ((*arr)->capacity * sizeof(t_vec4));
		(*arr)->capacity *= 2;
		if (!(new = create_4dvertices_array((*arr)->capacity)))
			return (NULL);
		ft_memcpy(new, *arr, old_size);
		free(*arr);
		*arr = new;
	}
	(*arr)->vertices[(*arr)->len++] = v;
	return (*arr);
}

t_4dvertices	*splice_4dvertices_array(t_4dvertices *arr, int index, int n)
{
	if (index < 0)
		index = index + arr->len - 1;
	if (n <= 0 || index >= arr->len || index + n > arr->len)
		return (NULL);
	ft_memcpy(arr->vertices + index, arr->vertices + index + n,
		(arr->len - (index + n)) * sizeof(t_vec4));
	arr->len -= n;
	return (arr);
}

t_4dvertices	*copy_4dvertices_array(t_4dvertices *src, t_4dvertices **dst)
{
	if ((*dst)->capacity < src->capacity)
	{
		free(*dst);
		if (!(*dst = create_4dvertices_array(src->capacity)))
			return (NULL);
	}
	ft_memcpy(*dst, src,
		sizeof(t_4dvertices) + (src->capacity * sizeof(t_vec4)));
	return (*dst);
}
