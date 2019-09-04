/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ptrs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/04 17:20:48 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/04 17:25:13 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include "arrays.h"

t_ptrs	*create_ptrs_array(int capacity)
{
	const size_t	size = sizeof(t_ptrs) + (capacity * sizeof(void *));
	t_ptrs	*arr;

	if (capacity <= 0)
		return (NULL);
	if (!(arr = (t_ptrs*)malloc(size)))
		return (NULL);
	ft_bzero(arr, size);
	arr->capacity = capacity;
	return (arr);
}

t_ptrs	*append_ptrs_array(t_ptrs **arr, void *p)
{
	t_ptrs	*new;
	size_t			old_size;

	if ((*arr)->len == (*arr)->capacity)
	{
		old_size = sizeof(t_ptrs) + ((*arr)->capacity * sizeof(void *));
		(*arr)->capacity *= 2;
		if (!(new = create_ptrs_array((*arr)->capacity)))
			return (NULL);
		ft_memcpy(new, *arr, old_size);
		free(*arr);
		*arr = new;
	}
	(*arr)->values[(*arr)->len++] = p;
	return (*arr);
}

t_ptrs	*splice_ptrs_array(t_ptrs *arr, int index, int n)
{
	if (index < 0)
		index = index + arr->len - 1;
	if (n <= 0 || index >= arr->len || index + n > arr->len)
		return (NULL);
	ft_memcpy(arr->values + index, arr->values + index + n,
		(arr->len - (index + n)) * sizeof(void *));
	arr->len -= n;
	return (arr);
}

t_ptrs	*copy_ptrs_array(t_ptrs *src, t_ptrs **dst)
{
	if ((*dst)->capacity < src->capacity)
	{
		free(*dst);
		if (!(*dst = create_ptrs_array(src->capacity)))
			return (NULL);
	}
	ft_memcpy(*dst, src,
		sizeof(t_ptrs) + (src->capacity * sizeof(void *)));
	return (*dst);
}
