/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arraylist_splice.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/28 19:41:45 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/29 01:52:11 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_arraylist	*arraylist_splicef(t_arraylist *arr, int index, int n, void (*del_val)(void *elem))
{
	int		i;

	if (index < 0)
		index = index + arr->len - 1;
	if (n <= 0 || index >= arr->len || index + n >= arr->len)
		return (NULL);
	i = index;
	while (del_val && i <= n)
		del_val(arr->values[i++]);
	ft_memcpy(arr->values + index, arr->values + index + n, (arr->len - (index + n)) * sizeof(void *));
	arr->len -= n;
	return (arr);
}

t_arraylist	*arraylist_splice(t_arraylist *arr, int index, int n)
{
	return (arraylist_splicef(arr, index, n, NULL));
}
