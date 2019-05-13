/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bsp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:42:14 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/11 23:19:20 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	free_polygons(t_polygons *polygons)
{
	t_polygon	*poly;

	int i = -1;
	while (++i < polygons->len)
	{
		poly = &polygons->polygons[i];
		if (poly->vertices)
			ft_memdel(&poly->vertices);
		if (poly->proj_vertices)
			ft_memdel(&poly->proj_vertices);
		if (poly->proj_vertices_buf)
			ft_memdel(&poly->proj_vertices_buf);
	}
}

void	free_node(t_node *n)
{
	if (n->front)
		free_node(n->front);
	if (n->back)
		free_node(n->back);
	if (n->polygons)
	{
		free_polygons(n->polygons);
		ft_memdel(&n->polygons);
	}
	ft_memdel(&n);
}