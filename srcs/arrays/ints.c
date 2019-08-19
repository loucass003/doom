/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ints.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 19:55:48 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/19 15:02:45 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_ints	*create_ints_array(int capacity)
{
	const size_t	size = sizeof(t_ints) + (capacity * sizeof(int));
	t_ints			*arr;

	if (capacity <= 0)
		return (NULL);
	if (!(arr = (t_ints*)malloc(size)))
		return (NULL);
	ft_bzero(arr, size);
	arr->capacity = capacity;
	return (arr);
}

t_ints	*append_ints_array(t_ints **arr, int i)
{
	t_ints			*new;
	size_t			old_size;

	if ((*arr)->len == (*arr)->capacity)
	{
		old_size = sizeof(t_ints) + ((*arr)->capacity * sizeof(int));
		(*arr)->capacity *= 2;
		if (!(new = create_ints_array((*arr)->capacity)))
			return (NULL);
		ft_memcpy(new, *arr, old_size);
		free(*arr);
		*arr = new;
	}
	(*arr)->values[(*arr)->len++] = i;
	return (*arr);
}

t_ints	*splice_ints_array(t_ints *arr, int index, int n)
{
	if (index < 0)
		index = index + arr->len - 1;
	if (n <= 0 || index >= arr->len || index + n > arr->len)
		return (NULL);
	ft_memcpy(arr->values + index, arr->values + index + n,
		(arr->len - (index + n)) * sizeof(int));
	arr->len -= n;
	return (arr);
}

t_ints	*copy_ints_array(t_ints *src, t_ints **dst)
{
	if ((*dst)->capacity < src->capacity)
	{
		free(*dst);
		if (!(*dst = create_ints_array(src->capacity)))
			return (NULL);
	}
	ft_memcpy(*dst, src,
		sizeof(t_ints) + (src->capacity * sizeof(int)));
	return (*dst);
}
