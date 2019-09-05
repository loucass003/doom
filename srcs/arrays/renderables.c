/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderables.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/05 00:11:08 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/05 00:16:00 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include "render.h"

t_renderables	*create_renderables_array(int capacity)
{
	const size_t	size = sizeof(t_renderables) + (capacity * sizeof(t_renderable));
	t_renderables	*arr;

	if (capacity <= 0)
		return (NULL);
	if (!(arr = (t_renderables*)malloc(size)))
		return (NULL);
	ft_bzero(arr, size);
	arr->capacity = capacity;
	return (arr);
}

t_renderables	*append_renderables_array(t_renderables **arr, t_renderable p)
{
	t_renderables	*new;
	size_t			old_size;

	if ((*arr)->len == (*arr)->capacity)
	{
		old_size = sizeof(t_renderables) + ((*arr)->capacity * sizeof(t_renderable));
		(*arr)->capacity *= 2;
		if (!(new = create_renderables_array((*arr)->capacity)))
			return (NULL);
		ft_memcpy(new, *arr, old_size);
		free(*arr);
		*arr = new;
	}
	(*arr)->values[(*arr)->len++] = p;
	return (*arr);
}

t_renderables	*splice_renderables_array(t_renderables *arr, int index, int n)
{
	if (index < 0)
		index = index + arr->len - 1;
	if (n <= 0 || index >= arr->len || index + n > arr->len)
		return (NULL);
	ft_memcpy(arr->values + index, arr->values + index + n,
		(arr->len - (index + n)) * sizeof(t_renderable));
	arr->len -= n;
	return (arr);
}

t_renderables	*copy_renderables_array(t_renderables *src, t_renderables **dst)
{
	if ((*dst)->capacity < src->capacity)
	{
		free(*dst);
		if (!(*dst = create_renderables_array(src->capacity)))
			return (NULL);
	}
	ft_memcpy(*dst, src,
		sizeof(t_renderables) + (src->capacity * sizeof(t_renderable)));
	return (*dst);
}
