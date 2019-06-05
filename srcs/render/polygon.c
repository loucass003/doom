/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 22:39:14 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/05 22:47:11 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	assemble_triangles(t_doom *doom, t_polygon *poly)
{
	const size_t	len = floorf(poly->indices->len / 3.);
	size_t			i;

	i = -1;
	while (++i < len)
	{
		float d = ft_vec3_dot(poly->normals[i], ft_vec3_sub(doom->player.pos, poly->vertices->vertices[poly->indices->values[i * 3]]));
		if (d <= 0 && poly->type != P_FLOOR)
			continue;
		process_triangle(doom, poly, (t_triangle){
			{poly->pp_vertices[poly->indices->values[i * 3]], poly->uvs[poly->indices->values[i * 3]]},
			{poly->pp_vertices[poly->indices->values[i * 3 + 1]], poly->uvs[poly->indices->values[i * 3 + 1]]},
			{poly->pp_vertices[poly->indices->values[i * 3 + 2]], poly->uvs[poly->indices->values[i * 3 + 2]]}
		});
	}
}

void	render_polygon(t_doom *doom, t_polygon *poly)
{
	int	i;

	i = -1;
	while (++i < poly->vertices->len)
		poly->pp_vertices[i] = mat4_mulv4(doom->player.matrix, vec3_to_4(poly->vertices->vertices[i]));
	assemble_triangles(doom, poly);
}
