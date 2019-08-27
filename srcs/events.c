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

t_bool test_collision(t_doom *doom, t_collision *hit, t_vec3 pos, t_vec3 dir, t_bool deep)
{
	// if (dir.x == 0 && dir.y == 0 && dir.z == 0)
	// 	return FALSE; 
	*hit = (t_collision) { .collide = FALSE, .dist = -1 };
	for (int i = 0; i < doom->polygons->len; i++)
	{
		t_polygon *poly = &doom->polygons->polygons[i];
		int triangles = floorf(poly->indices->len / 3.);
		for (int j = 0; j < triangles; j++)
		{
			t_collide_triangle tri = poly->collidables[j].data.triangle;
			t_vec3 p = ft_vec3_sub(pos, tri.points[0]);
			float dist = poly->type == P_WALL ? 0.25 : poly->type == P_CEILING ? 0.2 : 0.6;
			float d = ft_vec3_dot(poly->normals[j], p);
			if (d > 0 && d < dist)
			{
				t_vec3 n0 = ft_vec3_norm(ft_vec3_cross(poly->normals[j], ft_vec3_sub(tri.points[1], tri.points[0])));
				t_vec3 n1 = ft_vec3_norm(ft_vec3_cross(poly->normals[j], ft_vec3_sub(tri.points[1], tri.points[2])));
				t_vec3 n2 = ft_vec3_norm(ft_vec3_cross(poly->normals[j], ft_vec3_sub(tri.points[0], tri.points[2])));
				if (ft_vec3_dot(n0, p) > 0
					|| ft_vec3_dot(n1, p) > 0 
					|| ft_vec3_dot(n2, p) > 0)
					continue;
				t_vec3 newdir = ft_vec3_mul_s(poly->normals[j], ft_vec3_dot(dir, ft_vec3_inv(poly->normals[j])));
				//t_collision h;
				if (deep && test_collision(doom, hit, pos, newdir, FALSE))
					return (FALSE);
				hit->collide = TRUE;
				hit->dist = d;
				hit->normal = poly->normals[j];
				return (FALSE); //TODO: MEH
			}
		}
	}
	return TRUE;
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
	if (dir.x != 0 || dir.y != 0 || dir.z != 0)
	{
		update_maxtrix(doom);
		//int l = 0;
		t_collision hit;
		if (test_collision(doom, &hit, ft_vec3_add(doom->player.pos, dir), dir, TRUE))
		{
			doom->player.pos = ft_vec3_add(doom->player.pos, dir);
		}
		else
		{
			if (hit.collide)
				doom->player.pos = ft_vec3_add(doom->player.pos, dir);
			printf("HUM ?\n");
			t_vec3 newdir = ft_vec3_mul_s(hit.normal, ft_vec3_dot(dir, ft_vec3_inv(hit.normal)));
			doom->player.pos = ft_vec3_add(doom->player.pos, newdir);
		}
		

		// if (l >= 100)
		// {
		// 	doom->player.pos.x -= dir.x;
		// 	doom->player.pos.z -= dir.z;
		// 	doom->player.pos.y -= dir.y;
		// }
	}
	update_maxtrix(doom);
	while (SDL_PollEvent(&event))
		events_window(doom, &event);
	SDL_PumpEvents();
}
