/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 13:59:46 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/22 14:37:24 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include "obj.h"

t_objs	*create_objs_array(int capacity)
{
	const size_t	size = sizeof(t_objs) + (capacity * sizeof(t_obj));
	t_objs			*arr;

	if (capacity <= 0)
		return (NULL);
	if (!(arr = (t_objs*)malloc(size)))
		return (NULL);
	ft_bzero(arr, size);
	arr->capacity = capacity;
	return (arr);
}

t_objs	*append_objs_array(t_objs **arr, t_obj p)
{
	t_objs			*new;
	size_t			old_size;

	if ((*arr)->len == (*arr)->capacity)
	{
		old_size = sizeof(t_objs) + ((*arr)->capacity * sizeof(t_obj));
		(*arr)->capacity *= 2;
		if (!(new = create_objs_array((*arr)->capacity)))
			return (NULL);
		ft_memcpy(new, *arr, old_size);
		free(*arr);
		*arr = new;
	}
	(*arr)->objs[(*arr)->len++] = p;
	return (*arr);
}

t_objs	*splice_objs_array(t_objs *arr, int index, int n)
{
	if (index < 0)
		index = index + arr->len - 1;
	if (n <= 0 || index >= arr->len || index + n > arr->len)
		return (NULL);
	ft_memcpy(arr->objs + index, arr->objs + index + n,
		(arr->len - (index + n)) * sizeof(t_obj));
	arr->len -= n;
	return (arr);
}

t_objs	*copy_objs_array(t_objs *src, t_objs **dst)
{
	if ((*dst)->capacity < src->capacity)
	{
		free(*dst);
		if (!(*dst = create_objs_array(src->capacity)))
			return (NULL);
	}
	ft_memcpy(*dst, src,
		sizeof(t_objs) + (src->capacity * sizeof(t_obj)));
	return (*dst);
}
