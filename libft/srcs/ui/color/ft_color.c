/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_color.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/22 03:18:50 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/08 14:53:30 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_color		ft_color(uint8_t r, uint8_t g, uint8_t b)
{
	return (t_color){ r, g, b, 255 };
}

t_bool		ft_json_color(t_json_value *val, t_color *color)
{
	t_json_array	*arr;
	t_json_element	*elem;
	t_color_u		*c;
	int				i;

	if (!val || val->type != JSON_ARRAY)
		return (FALSE);
	arr = (t_json_array *)val;
	if (arr->elems_count != 3 && arr->elems_count != 4)
		return (FALSE);
	elem = arr->elements;
	i = 0;
	c = (t_color_u*)color;
	c->c.a = (uint8_t)255;
	while (elem)
	{
		if (elem->value->type != JSON_NUMBER)
			return (FALSE);
		c->a[i++] = (uint8_t)((t_json_number *)elem->value)->value;
		elem = elem->next;
	}
	return (TRUE);
}