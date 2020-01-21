/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scores.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/21 15:40:05 by louali            #+#    #+#             */
/*   Updated: 2020/01/21 15:41:29 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include "arrays.h"

t_scores	*create_scores_array(int capacity)
{
	const size_t	size = sizeof(t_scores) + (capacity * sizeof(t_scores));
	t_scores	*arr;

	if (capacity <= 0)
		return (NULL);
	if (!(arr = (t_scores*)malloc(size)))
		return (NULL);
	ft_bzero(arr, size);
	arr->capacity = capacity;
	return (arr);
}

t_scores	*append_scores_array(t_scores **arr, t_score v)
{
	t_scores	*new;
	size_t			old_size;

	if ((*arr)->len == (*arr)->capacity)
	{
		old_size = sizeof(t_scores) + ((*arr)->capacity * sizeof(t_score));
		(*arr)->capacity *= 2;
		if (!(new = create_scores_array((*arr)->capacity)))
			return (NULL);
		ft_memcpy(new, *arr, old_size);
		free(*arr);
		*arr = new;
	}
	(*arr)->values[(*arr)->len++] = v;
	return (*arr);
}

t_scores	*splice_scores_array(t_scores *arr, int index, int n)
{
	if (index < 0)
		index = index + arr->len - 1;
	if (n <= 0 || index >= arr->len || index + n > arr->len) //same as obj
		return (NULL);
	ft_memcpy(arr->values + index, arr->values + index + n,
		(arr->len - (index + n)) * sizeof(t_score));
	arr->len -= n;
	return (arr);
}

t_scores	*copy_scores_array(t_scores *src, t_scores **dst)
{
	if ((*dst)->capacity < src->capacity)
	{
		free(*dst);
		if (!(*dst = create_scores_array(src->capacity)))
			return (NULL);
	}
	ft_memcpy(*dst, src,
		sizeof(t_scores) + (src->capacity * sizeof(t_score)));
	return (*dst);
}

int		scores_indexof(t_scores *arr, t_score *elem)
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
