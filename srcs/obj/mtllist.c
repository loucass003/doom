/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtllist.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/27 22:50:34 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/27 23:40:56 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "obj.h"

t_mtllist	*create_mtllist(int capacity)
{
	const size_t	size = sizeof(t_mtllist) + (capacity * sizeof(t_mtl));
	t_mtllist		*arr;

	if (capacity <= 0)
		return (NULL);
	if (!(arr = (t_mtllist*)malloc(size)))
		return (NULL);
	ft_bzero(arr, size);
	arr->capacity = capacity;
	return (arr);
}

t_mtllist	*append_mtllist(t_mtllist **arr, t_mtl v)
{
	t_mtllist		*new;
	size_t			old_size;

	if ((*arr)->len == (*arr)->capacity)
	{
		old_size = sizeof(t_mtllist) + ((*arr)->capacity * sizeof(t_mtl));
		(*arr)->capacity *= 2;
		if (!(new = create_mtllist((*arr)->capacity)))
			return (NULL);
		ft_memcpy(new, *arr, old_size);
		free(*arr);
		*arr = new;
	}
	(*arr)->values[(*arr)->len++] = v;
	return (*arr);
}
