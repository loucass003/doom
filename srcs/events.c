/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 22:14:55 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/20 13:47:31 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static void	events_window(t_doom *doom, SDL_Event *event)
{
	const SDL_Scancode	key = event->key.keysym.scancode;

	if (doom->guis[doom->current_gui].on_event != NULL)
		doom->guis[doom->current_gui].on_event(&doom->guis[doom->current_gui], event, doom);
	if (event->type == SDL_QUIT)
		doom->running = FALSE;
	if (event->type == SDL_MOUSEBUTTONUP)
	{
		for (int i = 0; i < doom->guis[doom->current_gui].component_count; i++)
			doom->guis[doom->current_gui].components[i]
				->on_click(doom->guis[doom->current_gui].components[i],
				(t_vec2){ event->button.x, event->button.y }, doom);
	}
	if (event->type == SDL_MOUSEMOTION)
	{
		doom->mouse = (t_vec2){ event->motion.x, event->motion.y };
		for (int i = 0; i < doom->guis[doom->current_gui].component_count; i++)
		{
			doom->guis[doom->current_gui].components[i]
				->on_mouse_move(doom->guis[doom->current_gui].components[i],
				doom->mouse, doom);
		}
	}
	if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_P))
		doom->player.fixed_ray = !doom->player.fixed_ray;
	// if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_PAGEUP 
	// 	|| key == SDL_SCANCODE_PAGEDOWN || key == SDL_SCANCODE_T 
	// 	|| key == SDL_SCANCODE_G || key == SDL_SCANCODE_F 
	// 	|| key == SDL_SCANCODE_H))
	// 	room_map(doom, event, modify_room);
}

void	hook_events(t_doom *doom)
{
	const double	ms = doom->stats.delta * 3.;
	const Uint8		*s = SDL_GetKeyboardState(NULL);
	SDL_Event		event;

	if (s[SDL_SCANCODE_ESCAPE])
		doom->running = FALSE;
	t_vec3 dir = {0, 0, 0};
	if (s[SDL_SCANCODE_W] || s[SDL_SCANCODE_S])
	{
		dir.x += sinf(doom->player.rotation.y) * (s[SDL_SCANCODE_W] ? 1 : -1) * ms;
		dir.z += cosf(doom->player.rotation.y) * (s[SDL_SCANCODE_W] ? 1 : -1) * ms;
	}
	if (s[SDL_SCANCODE_A] || s[SDL_SCANCODE_D])
	{
		dir.x += -cosf(doom->player.rotation.y) * (s[SDL_SCANCODE_D] ? 1 : -1) * ms;
		dir.z += sinf(doom->player.rotation.y) * (s[SDL_SCANCODE_D] ? 1 : -1) * ms;
	} 
	if (s[SDL_SCANCODE_SPACE] || s[SDL_SCANCODE_LSHIFT])
		dir.y += (s[SDL_SCANCODE_SPACE] ? 1 : -1) * ms;
	if (s[SDL_SCANCODE_J] || s[SDL_SCANCODE_L])
		doom->player.rotation.y += 0.3 * (s[SDL_SCANCODE_J] ? 1 : -1) * ms;
	if (s[SDL_SCANCODE_I] || s[SDL_SCANCODE_K])
		doom->player.rotation.x += 0.3 * (s[SDL_SCANCODE_I] ? 1 : -1) * ms;
	t_vec3 old_pos = doom->player.pos;
	
	doom->player.pos.x += dir.x;
	doom->player.pos.z += dir.z;
	doom->player.pos.y += dir.y;
	
	

	if (dir.x != 0 || dir.y != 0 || dir.z != 0)
	{
		doom->player.ray = (t_ray){.origin = old_pos, .direction = ft_vec3_norm(dir)};
		t_ray ray = doom->player.ray;
		float dist = INT_MAX;
		for (int i = 0; i < doom->polygons->len; i++)
		{
			t_polygon *poly = &doom->polygons->polygons[i];
			int triangles = floorf(poly->indices->len / 3.);
			for (int j = 0; j < triangles; j++)
			{
				t_collision colision = ray_hit_collidable(&ray, poly->collidables + j);
				if (colision.collide && colision.dist != -1 && colision.dist < dist)
				{
					dist = colision.dist;
					if (dist < 0.25)
					{
						doom->player.pos = ft_vec3_add(doom->player.pos, ft_vec3_mul_s(poly->normals[j], 0.25 - dist));
						update_maxtrix(doom);
					}
					doom->player.pointed_poly = poly;
					doom->player.pointed_triangle = j;
				}
			}
		}
		if (dist == INT_MAX)
		{
			doom->player.pointed_triangle = -1;
			doom->player.pointed_poly = NULL;
		}
		update_maxtrix(doom);
	}
	update_maxtrix(doom);
	while (SDL_PollEvent(&event))
		events_window(doom, &event);
	SDL_PumpEvents();
}
