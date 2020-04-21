/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arraylist.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/28 19:34:12 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/01 01:22:34 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_arraylist	*create_arraylist(int capacity)
{
	t_arraylist			*arr;
	const size_t		size = sizeof(t_arraylist) + sizeof(void *) * capacity;

	if (capacity <= 0)
		return (NULL);
	if (!(arr = (t_arraylist *)malloc(size)))
		return (NULL);
	ft_bzero(arr, size);
	arr->capacity = capacity;
	return (arr);
}

void		arraylist_clear(t_arraylist **arr, void (*del_val)(void *elem))
{
	int		i;
	
	if (!(*arr))
		return;
	if (del_val)
	{
		i = -1;
		while (++i < (*arr)->len)
			del_val((*arr)->values[i]);
	}
	free(*arr);
	*arr = NULL;
}
