/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_size.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 01:13:35 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/01 15:28:20 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_bool		ft_json_size(t_json_value *val, t_size *size)
{
	t_json_array	*arr;
	t_json_element	*elem;
	t_size_u		*s;
	int				i;

	if (!(arr = json_to_array(val)) || arr->elems_count != 3)
		return (FALSE);
	elem = arr->elements;
	i = 0;
	s = (t_size_u*)size;
	while (elem)
	{
		if (elem->value->type != JSON_NUMBER)
			return (FALSE);
		s->a[i++] = (size_t)((t_json_number *)elem->value)->value;
		elem = elem->next;
	}
	return (TRUE);
}
