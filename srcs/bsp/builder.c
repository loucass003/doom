/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:36:38 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/14 21:09:45 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void		build_node(t_node *node)
{
	const int		len = fmax(1, node->polygons->len / 2);
	t_polygons		*front = create_polygons_array(len);
	t_polygons		*back = create_polygons_array(len);
	t_polygons		*colinear = create_polygons_array(len);
	int				i;

	if (node->type == N_LEAF)
	{
	/* 	ft_memdel(&front);
		ft_memdel(&colinear);
		ft_memdel(&back); */
		return ;
	}
	i = -1;
	while (++i < node->polygons->len)
	{
		t_polygon	*poly = &node->polygons->polygons[i];
		t_side		side = get_poly_side(node->partition, poly);
		if (side == S_COLINEAR)
			append_polygons_array(&colinear, *poly);
		else if (side == S_FRONT)
			append_polygons_array(&front, *poly);
		else if (side == S_BACK)
			append_polygons_array(&back, *poly);
		else if (side == S_SPANNING)
		{
			t_polygon	front_p;
			t_polygon	back_p;
			
			if (clip_poly(&front_p, poly, node->partition, S_BACK))
				append_polygons_array(&front, front_p);
			if (clip_poly(&back_p, poly, node->partition, S_FRONT))
				append_polygons_array(&back, back_p);
		//	free(poly->vertices);
		}
	}
	//free_polygons(node->polygons);
//	free(node->polygons);
//	arraylist_clear(&node->polygons, free_polygon);
	node->polygons = colinear;
	node->front = create_node(front);
	node->back = create_node(back);
	build_node(node->front);
	build_node(node->back);
}
