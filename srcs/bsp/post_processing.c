/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 21:24:43 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/05 23:57:01 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static t_bool	compute_normals(t_polygon *poly)
{
	const size_t	normals_count = floorf(poly->indices->len / 3.);
	const size_t	size = sizeof(t_vec3) * normals_count;
	size_t			i;

	if (!poly->normals && !(poly->normals = (t_vec3 *)malloc(size)))
		return (FALSE);
	if (!poly->uvs && !(poly->uvs = (t_vec2 *)malloc(poly->vertices->len * sizeof(t_vec2))))
		return (FALSE);
	if (!poly->pp_vertices && !(poly->pp_vertices = (t_vec4 *)malloc(poly->vertices->len * sizeof(t_vec4))))
		return (FALSE);
	i = -1;
	while (++i < normals_count)
	{
		poly->normals[i] = get_polygon_normal(poly);/*get_plane_normal(
			poly->vertices->vertices[poly->indices->values[i * 3]],
			poly->vertices->vertices[poly->indices->values[i * 3 + 1]],
			poly->vertices->vertices[poly->indices->values[i * 3 + 2]]);*/
	}
	i = -1;
	int len = floor(poly->indices->len / 3.);
	while (++i < len)
	{
		
		if (i % 2 == 0)
		{
			poly->uvs[poly->indices->values[i * 3]] = (t_vec2) { 1, 0 }; // 0 1
			poly->uvs[poly->indices->values[i * 3 + 1]] = (t_vec2) { 0, 0 }; // 1 1
			poly->uvs[poly->indices->values[i * 3 + 2]] = (t_vec2) { 1, 1 }; // 0 0
		}
		else
		{
			poly->uvs[poly->indices->values[i * 3]] = (t_vec2) { 1, 0 }; // 1 1
			poly->uvs[poly->indices->values[i * 3 + 1]] = (t_vec2) { 1, 1 }; // 0 0
			poly->uvs[poly->indices->values[i * 3 + 2]] = (t_vec2) { 0, 1 }; // 1 0
		}
		
	}
	return (TRUE);
}

t_bool			post_process_bsp(t_node *n)
{
	int	i;
	t_polygon	*poly;

	if (!n)
		return (TRUE);
	i = -1;
	while (++i < n->polygons->len)
	{
		poly = &n->polygons->polygons[i];
		if (!triangulate_polygon(poly) || !compute_normals(poly))
			return (FALSE);
	}
	post_process_bsp(n->front);
	post_process_bsp(n->back);
	return (TRUE);
}
