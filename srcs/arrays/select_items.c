/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select_items.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 23:06:40 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/11 10:19:09 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include "gui.h"

t_select_items	*create_select_items_array(int capacity)
{
	size_t			size;
	t_select_items	*arr;

	size = sizeof(t_select_items) + (capacity * sizeof(t_select_item));
	if (capacity <= 0)
		return (NULL);
	if (!(arr = (t_select_items*)malloc(size)))
		return (NULL);
	ft_bzero(arr, size);
	arr->capacity = capacity;
	arr->auto_free = TRUE;
	return (arr);
}

t_select_items	*append_select_items_array(t_select_items **arr,
	t_select_item v)
{
	t_select_items	*new;
	size_t			old_size;

	if ((*arr)->len == (*arr)->capacity)
	{
		old_size = sizeof(t_select_items)
			+ ((*arr)->capacity * sizeof(t_select_item));
		(*arr)->capacity *= 2;
		if (!(new = create_select_items_array((*arr)->capacity)))
			return (NULL);
		ft_memcpy(new, *arr, old_size);
		free(*arr);
		*arr = new;
	}
	(*arr)->values[(*arr)->len++] = v;
	return (*arr);
}

t_select_items	*splice_select_items_array(t_select_items *arr,
	int index, int n)
{
	if (index < 0)
		index = index + arr->len - 1;
	if (n <= 0 || index >= arr->len || index + n > arr->len)
		return (NULL);
	ft_memcpy(arr->values + index, arr->values + index + n,
		(arr->len - (index + n)) * sizeof(t_select_item));
	arr->len -= n;
	return (arr);
}

t_select_items	*copy_select_items_array(t_select_items *src,
	t_select_items **dst)
{
	if ((*dst)->capacity < src->capacity)
	{
		free(*dst);
		if (!(*dst = create_select_items_array(src->capacity)))
			return (NULL);
	}
	ft_memcpy(*dst, src,
		sizeof(t_select_items) + (src->capacity * sizeof(t_select_item)));
	return (*dst);
}

int				select_items_indexof(t_select_items *arr, int item_value)
{
	int	i;

	i = -1;
	while (++i < arr->len)
	{
		if (arr->values[i].value == item_value)
			return (i);
	}
	return (-1);
}
