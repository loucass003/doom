/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressources.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 13:56:22 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/22 14:41:09 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include "ressource.h"

t_ressources	*create_ressources_array(int capacity)
{
	size_t			size;
	t_ressources	*arr;

	size = sizeof(t_ressources) + (capacity * sizeof(t_ressource *));
	if (capacity <= 0)
		return (NULL);
	if (!(arr = (t_ressources*)malloc(size)))
		return (NULL);
	ft_bzero(arr, size);
	arr->capacity = capacity;
	return (arr);
}

t_ressources	*append_ressources_array(t_ressources **arr, t_ressource *v)
{
	t_ressources	*new;
	size_t			old_size;

	if (!v)
		return (NULL);
	if ((*arr)->len == (*arr)->capacity)
	{
		old_size = sizeof(t_ressources)
			+ ((*arr)->capacity * sizeof(t_ressource *));
		(*arr)->capacity *= 2;
		if (!(new = create_ressources_array((*arr)->capacity)))
			return (NULL);
		ft_memcpy(new, *arr, old_size);
		free(*arr);
		*arr = new;
	}
	(*arr)->values[(*arr)->len++] = v;
	return (*arr);
}

t_ressources	*splice_ressources_array(t_ressources *arr, int index, int n)
{
	if (index < 0)
		index = index + arr->len - 1;
	if (n <= 0 || index >= arr->len || index + n > arr->len)
		return (NULL);
	ft_memcpy(arr->values + index, arr->values + index + n,
		(arr->len - (index + n)) * sizeof(t_ressource *));
	arr->len -= n;
	return (arr);
}

t_ressources	*copy_ressources_array(t_ressources *src, t_ressources **dst)
{
	if ((*dst)->capacity < src->capacity)
	{
		free(*dst);
		if (!(*dst = create_ressources_array(src->capacity)))
			return (NULL);
	}
	ft_memcpy(*dst, src,
		sizeof(t_ressources) + (src->capacity * sizeof(t_ressource *)));
	return (*dst);
}

int				ressources_indexof(t_ressources *arr, t_ressource *r)
{
	int		j;

	j = -1;
	while (++j < arr->len)
		if (arr->values[j] == r)
			return (j);
	return (-1);
}
