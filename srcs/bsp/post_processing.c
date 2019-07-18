/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 21:24:43 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/17 03:12:24 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

#define GRID_WIDTH (30)
#define GRID_HEIGHT (30)

static t_bool	compute_normals(t_polygon *poly)
{
	const int		normals_count = floorf(poly->indices->len / 3.);
	const size_t	size = sizeof(t_vec3) * normals_count;
	int				i;

	if (!poly->normals && !(poly->normals = (t_vec3 *)malloc(size)))
		return (FALSE);
	if (!poly->pp_vertices && !(poly->pp_vertices = (t_vec4 *)malloc(poly->vertices->len * sizeof(t_vec4))))
		return (FALSE);
	i = -1;
	while (++i < normals_count)
		poly->normals[i] = get_polygon_normal(poly);
	return (TRUE);
}

t_bool			post_process_bsp(t_node *n, int x, int depth)
{
	int	i;
	t_polygon	*poly;

	if (!n)
		return (TRUE);
	n->depth = depth;
	i = -1;
	while (++i < n->polygons->len)
	{
		poly = &n->polygons->polygons[i];
		if (!poly->uvs && !(poly->uvs = (t_vec2 *)malloc(poly->vertices->len * sizeof(t_vec2))))
			return (FALSE);
		if (!triangulate_polygon(poly) || !compute_normals(poly))
			return (FALSE);
		compute_polygon_obb(poly);
	}
	post_process_bsp(n->front, x + GRID_WIDTH, depth + 1);
	post_process_bsp(n->back, x + GRID_WIDTH, depth + 1);
	return (TRUE);
}
