/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_sections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/12 16:31:36 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/22 14:45:30 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include "editor.h"

t_wall_sections	*create_wall_sections_array(int capacity)
{
	size_t			size;
	t_wall_sections	*arr;

	size = sizeof(t_wall_sections) + (capacity * sizeof(t_wall_section));
	if (capacity <= 0)
		return (NULL);
	if (!(arr = (t_wall_sections*)malloc(size)))
		return (NULL);
	ft_bzero(arr, size);
	arr->capacity = capacity;
	return (arr);
}

t_wall_sections	*append_wall_sections_array(t_wall_sections **arr,
	t_wall_section v)
{
	t_wall_sections	*new;
	size_t			old_size;

	if ((*arr)->len == (*arr)->capacity)
	{
		old_size = sizeof(t_wall_sections)
			+ ((*arr)->capacity * sizeof(t_wall_section));
		(*arr)->capacity *= 2;
		if (!(new = create_wall_sections_array((*arr)->capacity)))
			return (NULL);
		ft_memcpy(new, *arr, old_size);
		free(*arr);
		*arr = new;
	}
	(*arr)->values[(*arr)->len++] = v;
	return (*arr);
}

t_wall_sections	*splice_wall_sections_array(t_wall_sections *arr,
	int index, int n)
{
	if (index < 0)
		index = index + arr->len - 1;
	if (n <= 0 || index >= arr->len || index + n > arr->len)
		return (NULL);
	ft_memcpy(arr->values + index, arr->values + index + n,
		(arr->len - (index + n)) * sizeof(t_wall_section));
	arr->len -= n;
	return (arr);
}

t_wall_sections	*copy_wall_sections_array(t_wall_sections *src,
	t_wall_sections **dst)
{
	if ((*dst)->capacity < src->capacity)
	{
		free(*dst);
		if (!(*dst = create_wall_sections_array(src->capacity)))
			return (NULL);
	}
	ft_memcpy(*dst, src,
		sizeof(t_wall_sections) + (src->capacity * sizeof(t_wall_section)));
	return (*dst);
}

int				wall_sections_indexof(t_wall_sections *arr,
	t_wall_section *elem)
{
	int	i;

	i = -1;
	while (++i < arr->len)
	{
		if (arr->values + i == elem)
			return (i);
	}
	return (-1);
}
