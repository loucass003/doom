/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_ingame.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 11:22:28 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/02 19:33:20 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "doom.h"
#include <limits.h>
#include "gui.h"

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
	for (int i = 0; i < S_WIDTH * S_HEIGHT; i++)
		doom->main_context.buffer[i] = (t_zbuff){ 0, 0 };
	for (int i = 0; i < doom->polygons->len; i++)
		render_polygon(&doom->main_context, doom->polygons->polygons + i);
	// for (int i = 0; i < doom->polygons->len; i++)
	// {
	// 	if (doom->polygons->polygons[i].type != P_WALL)
	// 		continue;
	// 	t_vec3 p0 = doom->polygons->polygons[i].vertices->vertices[0];
	// 	t_vec3 p1 = doom->polygons->polygons[i].vertices->vertices[1];
	// 	t_vec2 a = (t_vec2){ p0.x * 10, p0.z * 10 }; 
	// 	t_vec2 b = (t_vec2){ p1.x * 10, p1.z * 10}; 
	// 	int color = doom->player.pointed_poly == doom->polygons->polygons + i ? 0xFFFF00FF : 0xFFFFFF00;
	// 	draw_line(&doom->screen, (t_pixel){ a.x, a.y, color }, (t_pixel){ b.x, b.y, 0xFFFFFFFF });
	// }
	
	// draw_circle(&doom->screen, (t_pixel){ S_WIDTH_2, S_HEIGHT_2, 0xFF00FFFF }, 4);
	// draw_circle(&doom->screen, (t_pixel){ doom->player.pos.x * 10 , doom->player.pos.z * 10, 0xFF00FFFF }, 4);
	// draw_line(&doom->screen, (t_pixel){ doom->player.ray.origin.x * 10 , doom->player.ray.origin.z * 10, 0xFF00FFFF }, (t_pixel){ doom->player.ray.origin.x * 10 + doom->player.ray.direction.x * 40, doom->player.ray.origin.z * 10 + doom->player.ray.direction.z * 40, 0xFF });
	
	// for (int i = 0; i < S_WIDTH * S_HEIGHT; i++)
	// {
	// 	t_ray_collide ray = doom->rays[i];
	// 	if (!ray.collision.collide)
	// 		continue;
	// 	draw_line(&doom->screen, (t_pixel){ ray.ray.origin.x * 10 , ray.ray.origin.z * 10, 0xFF00FFFF }, (t_pixel){ ray.ray.origin.x * 10 + ray.ray.direction.x * (ray.collision.dist * 10), ray.ray.origin.z * 10 + ray.ray.direction.z * (ray.collision.dist * 10), 0xFF });
	// }
	
	doom->obj_test->position = (t_vec3){1, 0, 2};
	doom->obj_test->rotation.y += 1 * doom->stats.delta;
	doom->obj_test->scale = (t_vec3){0.003, 0.003, 0.003};
 	doom->obj_test->dirty = TRUE;
	render_obj(&doom->main_context, doom->obj_test);
	
	//apply_image_to_image(&doom->screen, &doom->rt, (SDL_Rect){ 0, 0, doom->screen.width, doom->screen.height }, (SDL_Rect){ doom->screen.width - doom->rt.width / 5, doom->screen.height - doom->rt.height / 5, doom->rt.width / 5, doom->rt.height / 5 });
}
