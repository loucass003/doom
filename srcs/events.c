/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 22:14:55 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/15 15:46:49 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL.h>
#include <libft.h>
#include "collision.h"
#include "polygon.h"
#include "player.h"
#include <limits.h>
#include "doom.h"

static void	events_window(t_doom *doom, SDL_Event *event)
{
	const SDL_Scancode	key = event->key.keysym.scancode;

	if (doom->current_gui >= 0 && doom->guis[doom->current_gui].on_event != NULL)
		doom->guis[doom->current_gui].on_event(&doom->guis[doom->current_gui], event, doom);
	if (event->type == SDL_QUIT)
		doom->running = FALSE;
	if (event->type == SDL_MOUSEBUTTONUP && doom->current_gui >= 0)
	{
		for (int i = 0; i < doom->guis[doom->current_gui].component_count; i++)
			doom->guis[doom->current_gui].components[i]
				->on_click(doom->guis[doom->current_gui].components[i],
				(t_vec2){ event->button.x, event->button.y }, doom);
	}
	if (event->type == SDL_MOUSEMOTION && doom->current_gui >= 0)
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

t_bool test_collision(t_doom *doom)
{
	for (int i = 0; i < doom->renderables->len; i++)
	{
		t_renderable *r = &doom->renderables->values[i];
		// if (r->of.type != RENDERABLE_POLYGON)
		// 	continue;
		for (int j = 0; j < r->faces->len; j++)
		{
			t_face *face = &r->faces->values[j];
		//	t_polygon *poly = r->of.data.polygon;
			t_collide_triangle tri = face->pp_collidable.data.triangle;
			t_vec3 dir = ft_vec3_norm(doom->player.velocity);
			t_vec3 p = ft_vec3_sub(doom->player.pos, tri.points[0]);
		//	float dist = r->of.data.polygon->type == P_WALL ? 0.25 : poly->type == P_CEILING ? 0.2 : 0.6;
			float d = ft_vec3_dot(face->face_normal, p);
			//printf("dist %f\n", d);
			t_collision hit = triangle_hit_aabb(&tri, &doom->player.aabb);
			if (hit.collide)
			{
			/* 	printf("COLLIDE\n");
				printf("face_normal %f %f %f\n", face->face_normal.x, face->face_normal.y, face->face_normal.z);
				printf("tri_normal %f %f %f\n", tri.normal.x, tri.normal.y, tri.normal.z);
				printf("p %f %f %f\n", p.x, p.y, p.z);
				printf("p0 %f %f %f\n", tri.points[0].x, tri.points[0].y, tri.points[0].z);
				printf("p1 %f %f %f\n", tri.points[1].x, tri.points[1].y, tri.points[1].z);
				printf("p2 %f %f %f\n", tri.points[2].x, tri.points[2].y, tri.points[2].z); */
			//	t_vec3 n0 = ft_vec3_cross(face->face_normal, ft_vec3_norm(ft_vec3_sub(tri.points[1], tri.points[0])));
				//printf("n0 %f %f %f - %f\n", n0.x, n0.y, n0.z, ft_vec3_dot(n0, p));
			//	t_vec3 n1 = ft_vec3_cross(face->face_normal, ft_vec3_norm(ft_vec3_sub(tri.points[1], tri.points[2])));
			//	printf("n1 %f %f %f - %f\n", n1.x, n1.y, n1.z, ft_vec3_dot(n1, p));
			//	t_vec3 n2 = ft_vec3_cross(face->face_normal, ft_vec3_norm(ft_vec3_sub(tri.points[0], tri.points[2])));
			//	printf("n2 %f %f %f - %f\n", n2.x, n2.y, n2.z, ft_vec3_dot(n2, p));
				// if (ft_vec3_dot(n0, p) < 0
				// 	|| ft_vec3_dot(n1, p) < 0
				// 	|| ft_vec3_dot(n2, p) < 0)
				// 	continue;
				t_vec3 newdir = ft_vec3_mul_s(face->face_normal, ft_vec3_dot(doom->player.velocity, ft_vec3_inv(face->face_normal)));
			//	printf("pos %f %f %f\n", doom->player.pos.x, doom->player.pos.y, doom->player.pos.z);
				//doom->player.pos = ft_vec3_add(doom->player.pos, newdir);
			//	printf("new pos %f %f %f\n", doom->player.pos.x, doom->player.pos.y, doom->player.pos.z);
				doom->player.velocity = ft_vec3_add(doom->player.velocity, newdir);
				return TRUE;
			}
		}
	}
}

void	hook_events(t_doom *doom)
{
	const double	ms = doom->stats.delta * 3.;
	const Uint8		*s = SDL_GetKeyboardState(NULL);
	SDL_Event		event;

	if (s[SDL_SCANCODE_ESCAPE])
		doom->running = FALSE;
	if (s[SDL_SCANCODE_W] || s[SDL_SCANCODE_S])
	{
		doom->player.velocity.x += sinf(doom->player.rotation.y) * (s[SDL_SCANCODE_W] ? 1 : -1);
		doom->player.velocity.z += cosf(doom->player.rotation.y) * (s[SDL_SCANCODE_W] ? 1 : -1);
	}
	if (s[SDL_SCANCODE_A] || s[SDL_SCANCODE_D])
	{
		doom->player.velocity.x += -cosf(doom->player.rotation.y) * (s[SDL_SCANCODE_D] ? 1 : -1);
		doom->player.velocity.z += sinf(doom->player.rotation.y) * (s[SDL_SCANCODE_D] ? 1 : -1);
	} 
	if (s[SDL_SCANCODE_SPACE] || s[SDL_SCANCODE_LSHIFT])
		doom->player.velocity.y += (s[SDL_SCANCODE_SPACE] ? 1 : -1);
	if (s[SDL_SCANCODE_J] || s[SDL_SCANCODE_L])
		doom->player.rotation.y += 0.3 * (s[SDL_SCANCODE_J] ? 1 : -1) * ms;
	if (s[SDL_SCANCODE_I] || s[SDL_SCANCODE_K])
		doom->player.rotation.x += 0.3 * (s[SDL_SCANCODE_I] ? 1 : -1) * ms;
	// update_player_camera(&doom->player);
	test_collision(doom);
	doom->player.pos = ft_vec3_add(doom->player.pos, ft_vec3_mul_s(doom->player.velocity, ms));
	doom->player.velocity.y -= 0.05;
	doom->player.velocity = ft_vec3_mul_s(doom->player.velocity, 0.8);
	update_player_camera(&doom->player);
	
	while (SDL_PollEvent(&event))
		events_window(doom, &event);
	SDL_PumpEvents();
}
