/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 21:38:16 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/22 14:36:02 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include "light.h"

t_lights	*create_lights_array(int capacity)
{
	const size_t	size = sizeof(t_lights) + (capacity * sizeof(t_light));
	t_lights		*arr;

	if (capacity <= 0)
		return (NULL);
	if (!(arr = (t_lights*)malloc(size)))
		return (NULL);
	ft_bzero(arr, size);
	arr->capacity = capacity;
	return (arr);
}

t_lights	*append_lights_array(t_lights **arr, t_light p)
{
	t_lights		*new;
	size_t			old_size;

	if ((*arr)->len == (*arr)->capacity)
	{
		old_size = sizeof(t_lights) + ((*arr)->capacity * sizeof(t_light));
		(*arr)->capacity *= 2;
		if (!(new = create_lights_array((*arr)->capacity)))
			return (NULL);
		ft_memcpy(new, *arr, old_size);
		free(*arr);
		*arr = new;
	}
	(*arr)->values[(*arr)->len++] = p;
	return (*arr);
}

t_lights	*splice_lights_array(t_lights *arr, int index, int n)
{
	if (index < 0)
		index = index + arr->len - 1;
	if (n <= 0 || index >= arr->len || index + n > arr->len)
		return (NULL);
	ft_memcpy(arr->values + index, arr->values + index + n,
		(arr->len - (index + n)) * sizeof(t_light));
	arr->len -= n;
	return (arr);
}

t_lights	*copy_lights_array(t_lights *src, t_lights **dst)
{
	if ((*dst)->capacity < src->capacity)
	{
		free(*dst);
		if (!(*dst = create_lights_array(src->capacity)))
			return (NULL);
	}
	ft_memcpy(*dst, src,
		sizeof(t_lights) + (src->capacity * sizeof(t_light)));
	return (*dst);
}
