/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:36:38 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/03 16:56:44 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void		build_node(t_node *node)
{
	const int		len = fmax(1, node->polygons->len / 2);
	t_arraylist		*front = create_arraylist(len);
	t_arraylist		*back = create_arraylist(len);
	t_arraylist		*colinear = create_arraylist(len);
	int				i;

	if (node->type == N_LEAF)
	{
		// free(front);
		// free(colinear);
		// free(back);
		return ;
	}
	i = -1;
	while (++i < node->polygons->len)
	{
		t_polygon	*poly = (t_polygon *)node->polygons->values[i];
		t_side		side = get_poly_side(node->partition, poly);
		if (side == S_COLINEAR)
			arraylist_appendm(&colinear, poly, sizeof(t_polygon));
		else if (side == S_FRONT)
			arraylist_appendm(&front, poly, sizeof(t_polygon));
		else if (side == S_BACK)
			arraylist_appendm(&back, poly, sizeof(t_polygon));
		else if (side == S_SPANNING)
		{
			t_polygon	*front_p = clip_poly(poly, node->partition, S_BACK);
			t_polygon	*back_p = clip_poly(poly, node->partition, S_FRONT);
			if (front_p)
				arraylist_append(&front, front_p);
			if (back_p)
				arraylist_append(&back, back_p);
		}
	}
//	arraylist_clear(&node->polygons, free_polygon);
	node->polygons = colinear;
	node->front = create_node(front);
	node->back = create_node(back);
	build_node(node->front);
	build_node(node->back);
}
