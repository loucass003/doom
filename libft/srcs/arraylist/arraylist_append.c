/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arraylist_append.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/28 19:55:05 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/30 23:04:25 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_arraylist	*arraylist_append(t_arraylist **arr, void *elem)
{
	t_arraylist *new;
	size_t		old_size;

	if ((*arr)->len == (*arr)->capacity)
	{
		old_size = sizeof(t_arraylist) + sizeof(void *) * (*arr)->capacity;
		(*arr)->capacity *= 2;
		if (!(new = create_arraylist((*arr)->capacity)))
			return (NULL);
		ft_memcpy(new, *arr, old_size);
		free(*arr);
		*arr = new;
	}
	(*arr)->values[(*arr)->len++] = elem;
	return (*arr);
}

t_arraylist	*arraylist_appendm(t_arraylist **arr, void *elem, size_t n)
{
	void	*new;
	
	if (!(new = malloc(n)))
		return (NULL);
	ft_memcpy(new, elem, n);
	return (arraylist_append(arr, new));
}
