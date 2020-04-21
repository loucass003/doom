/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arraylist_indexof.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/28 19:42:47 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/29 01:52:08 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		arraylist_indexof(t_arraylist *arr, void *elem)
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
