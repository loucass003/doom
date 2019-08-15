/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 21:24:43 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/15 20:07:47 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_collidable	compute_triangle_collidable(t_polygon *poly, int triangle_index)
{
	t_collide_triangle		*tri;
	t_vec3					v;

	tri = (t_collide_triangle *)poly->collidables[triangle_index];
	tri->normal = poly->normals[triangle_index];
	tri->points[0] = poly->vertices->vertices[poly->indices->values[triangle_index * 3]];
	tri->points[1] = poly->vertices->vertices[poly->indices->values[triangle_index * 3 + 1]];
	tri->points[2] = poly->vertices->vertices[poly->indices->values[triangle_index * 3 + 2]];
	ft_memcpy(tri->points, poly->vertices->vertices + triangle_index * 3, sizeof(t_vec3) * 3);
	v = ft_vec3_sub(tri->points[1], tri->points[0]);
	tri->edge_normals[0] = ft_vec3_norm(ft_vec3_cross(tri->normal, v));
	v = ft_vec3_sub(tri->points[1], tri->points[2]);
	tri->edge_normals[1] = ft_vec3_norm(ft_vec3_cross(tri->normal, v));
	v = ft_vec3_sub(tri->points[0], tri->points[2]);
	tri->edge_normals[2] = ft_vec3_norm(ft_vec3_cross(tri->normal, v));
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
		poly->normals[i] = get_polygon_normal(poly);
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
