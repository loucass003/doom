/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 22:39:14 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/10 19:01:22 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	assemble_triangles(t_doom *doom, t_polygon *poly)
{
	const t_mtl		mtl = (t_mtl){
		.texture_map_set = TRUE,
		.texture_map = doom->textures.bricks
	};
	const size_t	len = floorf((poly->indices->len) / 3.);
	size_t			i;

	i = -1;
	while (++i < len)
	{
		float d = ft_vec3_dot(poly->normals[i], ft_vec3_sub(doom->player.pos, poly->vertices->vertices[poly->indices->values[i * 3]]));
		if (d <= 0 && poly->type != P_FLOOR)
			continue;
		process_triangle(doom, (t_mtl *)&mtl, (t_triangle){
			{poly->pp_vertices[poly->indices->values[i * 3]], poly->uvs[poly->indices->values[i * 3]]},
			{poly->pp_vertices[poly->indices->values[i * 3 + 1]], poly->uvs[poly->indices->values[i * 3 + 1]]},
			{poly->pp_vertices[poly->indices->values[i * 3 + 2]], poly->uvs[poly->indices->values[i * 3 + 2]]}
		});
	}
}

t_vec4	mat43_mulv4(t_mat4 m, t_vec4 p)
{
	t_vec4	r;

	r.x = p.x * m.a[0][0] + p.y * m.a[0][1] + p.z * m.a[0][2] + p.w * m.a[0][3];
	r.y = p.x * m.a[1][0] + p.y * m.a[1][1] + p.z * m.a[1][2] + p.w * m.a[1][3];
	r.z = p.x * m.a[2][0] + p.y * m.a[2][1] + p.z * m.a[2][2] + p.w * m.a[2][3];
	r.w = 0;
	//r.w = p.x * m.a[3][0] + p.y * m.a[3][1] + p.z * m.a[3][2] + p.w * m.a[3][3];
	return (r);
}

void	render_polygon(t_doom *doom, t_polygon *poly)
{
	int	i;

	i = -1;
	while (++i < poly->vertices->len)
		poly->pp_vertices[i] = mat43_mulv4(doom->player.matrix, vec3_to_4(poly->vertices->vertices[i]));
	assemble_triangles(doom, poly);
}
