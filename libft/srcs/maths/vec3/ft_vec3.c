/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/24 16:11:37 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/24 16:43:28 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_bool		ft_json_vec3(t_json_value *val, t_vec3 *vec)
{
	t_json_array	*arr;
	t_json_element	*elem;
	t_vec3_u		*v;
	int				i;

	if (!(arr = json_to_array(val)) || arr->elems_count != 3)
		return (FALSE);
	elem = arr->elements;
	i = 0;
	v = (t_vec3_u*)vec;
	while (elem)
	{
		if (elem->value->type != JSON_NUMBER)
			return (FALSE);
		v->a[i++] = (float)((t_json_number *)elem->value)->value;
		elem = elem->next;
	}
	return (TRUE);
}
