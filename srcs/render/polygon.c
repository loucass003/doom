/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 22:39:14 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/07 18:26:37 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#define NEAR (0.1)

void	render_polygon(t_doom *doom, t_polygon *poly)
{
	t_mat4 rotation = ft_mat4_rotation((t_vec3){0, doom->player.rotation, 0});
	int i = -1;
	while (++i < poly->vertices->len)
	{
		int next = (i + 1) % poly->vertices->len;
		// t_vec3 a = ft_mat4_mulv(rotation, ft_vec3_sub(*(t_vec3 *)(poly->vertices->values[i]), doom->player.pos));
	    // t_vec3 b = ft_mat4_mulv(rotation, ft_vec3_sub(*(t_vec3 *)(poly->vertices->values[next]), doom->player.pos));
		t_vec3 a = ft_mat4_mulv(doom->player.matrix, *(t_vec3 *)(poly->vertices->values[i]));
		t_vec3 b = ft_mat4_mulv(doom->player.matrix, *(t_vec3 *)(poly->vertices->values[next]));
		
		if (a.z < NEAR) {
			t_vec3 dir = ft_vec3_sub(a, b);
			float t = (NEAR - b.z) / dir.z;
			a = ft_vec3_add(ft_vec3_mul(dir, (t_vec3) { t, t, t }), b);
		}

		if (b.z < NEAR) {
			t_vec3 dir = ft_vec3_sub(a, b);
			float t = (NEAR - a.z) / dir.z;
			b = ft_vec3_add(ft_vec3_mul(dir, (t_vec3) { t, t, t }), a);
		}

		a = ft_mat4_mulv(doom->player.projection, a);
		b = ft_mat4_mulv(doom->player.projection, b);

		t_pixel p = (t_pixel){
			((a.x + 0.5) / a.z) * S_WIDTH,
			S_HEIGHT_2 + (a.y / a.z) * S_HEIGHT + (poly->type == P_FLOOR), 
			poly->type == P_FLOOR ? 0x00FF00 : 0xFF0000
		};
		t_pixel p2 = (t_pixel){
			((b.x + 0.5) / b.z) * S_WIDTH,
			S_HEIGHT_2 + (b.y / b.z) * S_HEIGHT + (poly->type == P_FLOOR),
			0xFF0000
		};
		draw_line(&doom->screen, p, p2);
	}
}
