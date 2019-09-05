/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon_post_process.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 16:26:29 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/05 01:29:14 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "polygon.h"
#include "maths/triangle.h"
#include <stdlib.h>
#include "doom.h"

// void			compute_triangle_collidable(t_polygon *poly, int triangle_index, t_collidable *c)
// {
// 	t_collide_triangle	*tri;
// 	if (c->type == COLLIDE_TRIANGLE)
// 	{
// 		tri = (t_collide_triangle *)&c->data.triangle;
// 		tri->points[0] = poly->vertices->vertices[poly->indices->values[triangle_index * 3]];
// 		tri->points[1] = poly->vertices->vertices[poly->indices->values[triangle_index * 3 + 1]];
// 		tri->points[2] = poly->vertices->vertices[poly->indices->values[triangle_index * 3 + 2]];
// 		tri->uv[0] = poly->uvs[poly->indices->values[triangle_index * 3]];
// 		tri->uv[1] = poly->uvs[poly->indices->values[triangle_index * 3 + 1]];
// 		tri->uv[2] = poly->uvs[poly->indices->values[triangle_index * 3 + 2]];
// 		tri->v0v1 = ft_vec3_sub(tri->points[1], tri->points[0]);
// 		tri->v0v2 = ft_vec3_sub(tri->points[2], tri->points[0]);
// 		tri->normal = poly->normals[triangle_index];
// 		tri->triangle = triangle_index;
// 		tri->parent_type = PARENT_COLLIDER_POLYGON;
// 		tri->polygon = poly;
// 	}
// }

// static t_bool	compute_normals(t_polygon *poly)
// {
// 	const int		normals_count = poly->indices->len / 3;
// 	int				i;

// 	if (!poly->normals && !(poly->normals = (t_vec3 *)malloc(sizeof(t_vec3) * normals_count)))
// 		return (FALSE);
// 	if (!poly->pp_vertices && !(poly->pp_vertices = (t_vec4 *)malloc(poly->vertices->len * sizeof(t_vec4))))
// 		return (FALSE);
// 	if (!poly->collidables && !(poly->collidables = (t_collidable *)malloc(normals_count * sizeof(t_collidable))))
// 		return (FALSE);
	
//  	i = -1;
// 	while (++i < normals_count)
// 	{
// 		poly->normals[i] = get_triangle_normal(
// 			poly->vertices->vertices[poly->indices->values[i * 3 + 2]],
// 			poly->vertices->vertices[poly->indices->values[i * 3 + 1]],
// 			poly->vertices->vertices[poly->indices->values[i * 3 + 0]]
// 		);
// 		poly->collidables[i].type = COLLIDE_TRIANGLE;
// 		compute_triangle_collidable(poly, i, poly->collidables + i);
// 	}
// 	return (TRUE);
// }

// t_bool			post_process_polygons(t_doom *doom)
// {
// 	int			i;
// 	t_polygon	*poly;

// 	i = -1;
// 	while (++i < doom->polygons->len) 
// 	{
// 		poly = &doom->polygons->polygons[i];
// 		if (!poly->uvs && !(poly->uvs = (t_vec2 *)malloc(poly->vertices->len * sizeof(t_vec2))))
// 			return (FALSE);
// 		if (!triangulate_polygon(poly) || !compute_normals(poly))
// 			return (FALSE);
// 	}
// 	return (TRUE);
// }

t_bool			post_process_polygon(t_polygon *poly)
{
/* 	if (!poly->uvs && !(poly->uvs = (t_vec2 *)malloc(poly->vertices->len * sizeof(t_vec2))))
		return (FALSE);
	if (!triangulate_polygon(poly) || !compute_normals(poly))
		return (FALSE); */
	return (TRUE);
}

