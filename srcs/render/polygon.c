/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 22:39:14 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/06 15:08:51 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_vec2	project_vertice(t_doom *doom, t_vec3 *vertice)
{
	t_vec3 v = ft_mat4_mulv(ft_mat4_rotation((t_vec3){0, doom->player.rotation, 0}), ft_vec3_sub(*vertice, doom->player.pos));
	//t_vec3 v = ft_mat4_mulv(doom->player.matrix, *vertice);
	if (v.z <= 0)
		v.z = -v.z;
	t_vec2 p = (t_vec2) {
		(v.x / v.z) * S_WIDTH,
		S_HEIGHT_2 + (v.y / v.z) * S_HEIGHT
	};
	return (p);
}

void	project_polygon(t_doom *doom, t_polygon *poly)
{
	int	i;

	if (!poly->proj_vertices && !(poly->proj_vertices = (t_vec2 *)malloc(sizeof(t_vec2) * poly->vertices->len)))
		return ; //TODO: ERROR
	i = -1;
	while (++i < poly->vertices->len)
		poly->proj_vertices[i] = project_vertice(doom, poly->vertices->values[i]);
}

void	render_polygon(t_doom *doom, t_polygon *poly)
{
	project_polygon(doom, poly);
	int i = -1;
	while (++i < poly->vertices->len)
	{
		int next = (i + 1) % poly->vertices->len;
		t_pixel p = (t_pixel){poly->proj_vertices[i].x, poly->proj_vertices[i].y + (poly->type == P_FLOOR),  poly->type == P_FLOOR ? 0x00FF00 : 0xFF0000};
		t_pixel p2 = (t_pixel){poly->proj_vertices[next].x, poly->proj_vertices[next].y + (poly->type == P_FLOOR), 0xFF0000};
		draw_line(&doom->screen, p, p2);
	}
}
