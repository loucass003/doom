/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   components.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 23:06:40 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/26 03:45:22 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include "gui.h"

t_components	*create_components_array(int capacity)
{
	const size_t	size = sizeof(t_components) + (capacity * sizeof(t_component *));
	t_components	*arr;

	if (capacity <= 0)
		return (NULL);
	if (!(arr = (t_components*)malloc(size)))
		return (NULL);
	ft_bzero(arr, size);
	arr->capacity = capacity;
	return (arr);
}

t_components	*append_components_array(t_components **arr, t_component *v)
{
	t_components	*new;
	size_t			old_size;

	if (!v)
		return (NULL);
	if ((*arr)->len == (*arr)->capacity)
	{
		old_size = sizeof(t_components) + ((*arr)->capacity * sizeof(t_component *));
		(*arr)->capacity *= 2;
		if (!(new = create_components_array((*arr)->capacity)))
			return (NULL);
		ft_memcpy(new, *arr, old_size);
		free(*arr);
		*arr = new;
	}
	(*arr)->values[(*arr)->len++] = v;
	return (*arr);
}

t_components	*splice_components_array(t_components *arr, int index, int n)
{
	if (index < 0)
		index = index + arr->len - 1;
	if (n <= 0 || index >= arr->len || index + n > arr->len)
		return (NULL);
	ft_memcpy(arr->values + index, arr->values + index + n,
		(arr->len - (index + n)) * sizeof(t_component *));
	arr->len -= n;
	return (arr);
}

t_components	*copy_components_array(t_components *src, t_components **dst)
{
	if ((*dst)->capacity < src->capacity)
	{
		free(*dst);
		if (!(*dst = create_components_array(src->capacity)))
			return (NULL);
	}
	ft_memcpy(*dst, src,
		sizeof(t_components) + (src->capacity * sizeof(t_component *)));
	return (*dst);
}

int		components_indexof(t_components *arr, t_component *elem)
{
	int	i;

	i = -1;
	while (++i < arr->len)
	{
		if (arr->values[i] == elem)
			return (i);
	}
	return (-1);
}
