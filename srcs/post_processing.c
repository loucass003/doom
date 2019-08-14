/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 21:24:43 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/14 13:53:31 by llelievr         ###   ########.fr       */
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
	t_vec3			v;

	if (!poly->normals && !(poly->normals = (t_vec3 *)malloc(size)))
		return (FALSE);
	if (!poly->pp_vertices && !(poly->pp_vertices = (t_vec4 *)malloc(poly->vertices->len * sizeof(t_vec4))))
		return (FALSE);
	if (!poly->colisions_normals && !(poly->colisions_normals = (t_vec3 *)malloc(size * 3)))
		return (FALSE);
	if (!poly->colision_dist && !(poly->colision_dist = (float *)malloc(normals_count * sizeof(float))))
		return (FALSE);
	ft_memset(poly->colision_dist, 255, normals_count * sizeof(float));
	i = -1;
	while (++i < normals_count)
	{
		poly->normals[i] = get_polygon_normal(poly);
		v = ft_vec3_sub(poly->vertices->vertices[poly->indices->values[i * 3 + 1]],
			poly->vertices->vertices[poly->indices->values[i * 3]]);
		poly->colisions_normals[i * 3] = ft_vec3_norm(ft_vec3_cross(poly->normals[i], v));
		v = ft_vec3_sub(poly->vertices->vertices[poly->indices->values[i * 3 + 1]],
			poly->vertices->vertices[poly->indices->values[i * 3 + 2]]);
		poly->colisions_normals[i * 3 + 1] = ft_vec3_norm(ft_vec3_cross(poly->normals[i], v));
		v = ft_vec3_sub(poly->vertices->vertices[poly->indices->values[i * 3]],
			poly->vertices->vertices[poly->indices->values[i * 3 + 2]]);
		poly->colisions_normals[i * 3 + 2] = ft_vec3_norm(ft_vec3_cross(poly->normals[i], v));
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
