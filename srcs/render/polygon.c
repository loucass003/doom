/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 22:39:14 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/29 03:04:51 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "maths/vec4.h"
#include "maths/triangle.h"
#include "doom.h"

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

typedef union ut_color {
	uint32_t color;
	struct argbTag
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
        uint8_t a;
	} argb;
	t_color	color_t;
} ur_color;

float t = 0;

void	render_polygon(t_doom *doom, t_polygon *poly)
{
	t_mtl		mtl = (t_mtl){
		.texture_map_set = TRUE,
		.texture_map = doom->textures.bricks
	};
	const size_t	len = floorf((poly->indices->len) / 3.);
	size_t			i;

	i = -1;
	while (++i < len)
	{
		float d = ft_vec3_dot(poly->normals[i], ft_vec3_sub(doom->player.pos, poly->vertices->vertices[poly->indices->values[i * 3]]));
		if (d <= 0)
			continue;
		if (poly == doom->player.pointed_poly)
		{
			mtl.material_color_set = TRUE;
			mtl.material_color = doom->player.pointed_triangle == i ? 0xFF00FF25 : 0xFF0250FF;
		}
		t += 0.01 * doom->stats.delta;
		float it = fmax(0, ft_vec3_dot(ft_vec3_inv(poly->normals[i]), ft_vec3_sub((t_vec3){1, 0, 1}, (t_vec3){ 0, 0, cos(t) * 3 })));
		
	
		it = fmax(0, fmin(1, 0.2 + it)) * 255;
		float light_color = it;
		process_triangle(doom, (t_mtl *)&mtl, (t_triangle){
			{.pos = mat43_mulv4(doom->player.matrix, vec3_to_4(poly->vertices->vertices[poly->indices->values[i * 3]])), .tex = poly->uvs[poly->indices->values[i * 3]], .normal = poly->normals[i], .light_color = light_color},
			{.pos = mat43_mulv4(doom->player.matrix, vec3_to_4(poly->vertices->vertices[poly->indices->values[i * 3 + 1]])), .tex = poly->uvs[poly->indices->values[i * 3 + 1]], .normal = poly->normals[i + 1], .light_color = light_color},
			{.pos = mat43_mulv4(doom->player.matrix, vec3_to_4(poly->vertices->vertices[poly->indices->values[i * 3 + 2]])), .tex = poly->uvs[poly->indices->values[i * 3 + 2]], .normal = poly->normals[i + 2], .light_color = light_color}
		});
	}
}
