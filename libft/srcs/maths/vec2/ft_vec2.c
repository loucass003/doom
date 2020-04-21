/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/24 16:30:10 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/01 15:29:51 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_bool		ft_json_vec2(t_json_value *val, t_vec2 *vec)
{
	t_json_array	*arr;
	t_json_element	*elem;
	t_vec2_u		*v;
	int				i;

	if (!(arr = json_to_array(val)) || arr->elems_count != 2)
		return (FALSE);
	elem = arr->elements;
	i = 0;
	v = (t_vec2_u*)vec;
	while (elem)
	{
		if (elem->value->type != JSON_NUMBER)
			return (FALSE);
		v->a[i++] = (float)((t_json_number *)elem->value)->value;
		elem = elem->next;
	}
	return (TRUE);
}
