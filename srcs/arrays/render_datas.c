/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_datas.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 14:44:37 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/27 15:04:54 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

t_render_datas	*create_render_data_array(int capacity)
{
	const size_t	size = sizeof(t_render_datas) + (capacity * sizeof(t_render_data));
	t_render_datas			*arr;

	if (capacity <= 0)
		return (NULL);
	if (!(arr = (t_render_datas*)malloc(size)))
		return (NULL);
	ft_bzero(arr, size);
	arr->capacity = capacity;
	return (arr);
}

t_render_datas	*append_render_data_array(t_render_datas **arr, t_render_data v)
{
	t_render_datas			*new;
	size_t			old_size;

	if ((*arr)->len == (*arr)->capacity)
	{
		old_size = sizeof(t_render_datas) + ((*arr)->capacity * sizeof(t_render_data));
		(*arr)->capacity *= 2;
		if (!(new = create_render_data_array((*arr)->capacity)))
			return (NULL);
		ft_memcpy(new, *arr, old_size);
		free(*arr);
		*arr = new;
	}
	(*arr)->values[(*arr)->len++] = v;
	return (*arr);
}

t_render_datas	*splice_render_data_array(t_render_datas *arr, int index, int n)
{
	if (index < 0)
		index = index + arr->len - 1;
	if (n <= 0 || index >= arr->len || index + n > arr->len)
		return (NULL);
	ft_memcpy(arr->values + index, arr->values + index + n,
		(arr->len - (index + n)) * sizeof(t_render_data));
	arr->len -= n;
	return (arr);
}

int		render_datas_indexof(t_render_datas *arr, t_render_data *elem)
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
