/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_ingame.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 11:22:28 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/20 14:54:56 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void static	action_performed(t_component *cmp, t_doom *doom)
{
	if (cmp == doom->guis[doom->current_gui].components[0])
	{
		set_gui(doom, GUI_EDITOR);
	}
}

void	g_ingame_on_enter(t_gui *self, t_doom *doom)
{
	
}

void	g_ingame_render(t_gui *self, t_doom *doom)
{
	for (int i = 0; i < (int)doom->screen.width * doom->screen.height; i++)
		doom->rendered_area[i] = 0;
	for (int i = 0; i < doom->polygons->len; i++)
		render_polygon(doom, doom->polygons->polygons + i);

	for (int i = 0; i < doom->polygons->len; i++)
	{
		if (doom->polygons->polygons[i].type != P_WALL)
			continue;
		t_vec3 p0 = doom->polygons->polygons[i].vertices->vertices[0];
		t_vec3 p1 = doom->polygons->polygons[i].vertices->vertices[1];
		t_vec2 a = (t_vec2){ p0.x * 10, p0.z * 10 }; 
		t_vec2 b = (t_vec2){ p1.x * 10, p1.z * 10}; 
		int color = doom->player.pointed_poly == doom->polygons->polygons + i ? 0xFF00FF : 0xFFFF00;
		draw_line(&doom->screen, (t_pixel){ a.x, a.y, color }, (t_pixel){ b.x, b.y, 0xFFFF });
	}
	draw_circle(&doom->screen, (t_pixel){ S_WIDTH_2, S_HEIGHT_2, 0xFF }, 4);
	draw_circle(&doom->screen, (t_pixel){ doom->player.pos.x * 10 , doom->player.pos.z * 10, 0xFF }, 4);
	draw_line(&doom->screen, (t_pixel){ doom->player.ray.origin.x * 10 , doom->player.ray.origin.z * 10, 0xFF }, (t_pixel){ doom->player.ray.origin.x * 10 + doom->player.ray.direction.x * 40, doom->player.ray.origin.z * 10 + doom->player.ray.direction.z * 40, 0xFF });
	doom->obj_test->position = (t_vec3){2, 0, 2};
	doom->obj_test->rotation.y += 3 * doom->stats.delta;
	doom->obj_test->scale = (t_vec3){0.002, 0.002, 0.002};
	// doom->obj_test->dirty = TRUE;
	render_obj(doom, doom->obj_test);
	
}
