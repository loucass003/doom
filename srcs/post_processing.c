/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 21:24:43 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/29 03:02:37 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "polygon.h"
#include "maths/triangle.h"
#include "doom.h"

void			compute_triangle_collidable(t_polygon *poly, int triangle_index, t_collidable *c)
{
	t_collide_triangle	*tri;
	if (c->type == COLLIDE_TRIANGLE)
	{
		tri = (t_collide_triangle *)&c->data.triangle;
		tri->points[0] = poly->vertices->vertices[poly->indices->values[triangle_index * 3]];
		tri->points[1] = poly->vertices->vertices[poly->indices->values[triangle_index * 3 + 1]];
		tri->points[2] = poly->vertices->vertices[poly->indices->values[triangle_index * 3 + 2]];
	}
}

static t_bool	compute_normals(t_polygon *poly)
{
	const int		normals_count = floorf(poly->indices->len / 3.);
	const size_t	size = sizeof(t_vec3) * normals_count;
	int				i;

	if (!poly->normals && !(poly->normals = (t_vec3 *)malloc(size)))
		return (FALSE);
	if (!poly->pp_vertices && !(poly->pp_vertices = (t_vec4 *)malloc(poly->vertices->len * sizeof(t_vec4))))
		return (FALSE);
	if (!poly->collidables && !(poly->collidables = (t_collidable *)malloc(normals_count * sizeof(t_collidable))))
		return (FALSE);
	
 	i = -1;
	while (++i < normals_count)
	{
		poly->normals[i] = get_triangle_normal(
			poly->vertices->vertices[poly->indices->values[i * 3 + 2]],
			poly->vertices->vertices[poly->indices->values[i * 3 + 1]],
			poly->vertices->vertices[poly->indices->values[i * 3 + 0]]
		);
		poly->collidables[i].type = COLLIDE_TRIANGLE;
	//	printf("n %f %f %f\n", poly->normals[i].x, poly->normals[i].y, poly->normals[i].z);
		compute_triangle_collidable(poly, i, &poly->collidables[i]);
	}
	return (TRUE);
}

t_bool			post_process_polygons(t_doom *doom)
{
	int			i;
	t_polygon	*poly;

	i = -1;
	while (++i < doom->polygons->len) 
	{
		poly = &doom->polygons->polygons[i];
		if (!poly->uvs && !(poly->uvs = (t_vec2 *)malloc(poly->vertices->len * sizeof(t_vec2))))
			return (FALSE);
		if (!triangulate_polygon(poly) || !compute_normals(poly))
			return (FALSE);
	}
	printf("CALLL POST PROCESS\n\n");
	return (TRUE);
}
