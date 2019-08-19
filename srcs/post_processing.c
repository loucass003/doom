/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 21:24:43 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/19 18:55:42 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_collidable	compute_triangle_collidable(t_polygon *poly, int triangle_index)
{
	t_collide_triangle		*tri;
	t_vec3					v;

	tri = (t_collide_triangle *)&poly->collidables[triangle_index];
	tri->super.type = COLLIDE_TRIANGLE;
	tri->normal = poly->normals[triangle_index];
	tri->points[0] = poly->vertices->vertices[poly->indices->values[triangle_index * 3]];
	tri->points[1] = poly->vertices->vertices[poly->indices->values[triangle_index * 3 + 1]];
	tri->points[2] = poly->vertices->vertices[poly->indices->values[triangle_index * 3 + 2]];
	printf("p0 %f %f %f\n", tri->points[0].x, tri->points[0].y, tri->points[0].z);
	printf("p1 %f %f %f\n", tri->points[1].x, tri->points[1].y, tri->points[1].z);
	printf("p2 %f %f %f\n", tri->points[2].x, tri->points[2].y, tri->points[2].z);
	t_vec3 n = get_triangle_normal(
			tri->points[2],
			tri->points[1],
			tri->points[0]
		);
	printf("n %f %f %f\n", n.x, n.y, n.z);
	printf("----------- %d\n", triangle_index);
	// v = ft_vec3_sub(tri->points[1], tri->points[0]);
	// tri->edge_normals[0] = ft_vec3_norm(ft_vec3_cross(tri->normal, v));
	// v = ft_vec3_sub(tri->points[1], tri->points[2]);
	// tri->edge_normals[1] = ft_vec3_norm(ft_vec3_cross(tri->normal, v));
	// v = ft_vec3_sub(tri->points[0], tri->points[2]);
	// tri->edge_normals[2] = ft_vec3_norm(ft_vec3_cross(tri->normal, v));
	return (*(t_collidable *)tri);
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
	if (!poly->collidables && !(poly->collidables = (t_collidable *)malloc(normals_count * sizeof(t_collide_triangle))))
		return (FALSE);
	
	i = -1;
	while (++i < normals_count)
	{
		poly->normals[i] = get_triangle_normal(
			poly->vertices->vertices[poly->indices->values[i * 3 + 2]],
			poly->vertices->vertices[poly->indices->values[i * 3 + 1]],
			poly->vertices->vertices[poly->indices->values[i * 3 + 0]]
		);
		printf("n %f %f %f\n", poly->normals[i].x, poly->normals[i].y, poly->normals[i].z);
		poly->collidables[i] = compute_triangle_collidable(poly, i);
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
}
