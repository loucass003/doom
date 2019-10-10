/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 22:14:55 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/10 04:49:50 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL.h>
#include <libft.h>
#include "collision.h"
#include "polygon.h"
#include "player.h"
#include <limits.h>
#include "octree.h"
#include "sprite.h"
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
	{
		t_ray ray = create_shoot_ray(doom->player, (t_vec3){0, 0, 1});
			t_collision min = (t_collision) {
				.collide = FALSE,
				.dist = INT_MAX
			};
			for (int i = 0; i < doom->renderables->len; i++)
			{
				t_renderable *r = &doom->renderables->values[i];
				if (!r->octree)
					continue;
				t_ray local = to_local_ray(ray, r->position, r->rotation, r->scale);
				ray.to_local = &local;
				ray_intersect_octree(r->octree, r, &ray, &min);
			}
			if (min.collide)
			{
				t_renderable *r = min.renderable;
				if (r && !r->entity && !r->wireframe)
				{
					t_renderable enemy;
					create_enemy(doom, &enemy);
					enemy.entity->position = min.point;
					enemy.entity->position.y += enemy.entity->radius.y;
					append_renderables_array(&doom->renderables, enemy);
				}
			}
	}

	if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_O))
	{
		if (doom->renderables->values[1].rotation.y != 0)
			doom->renderables->values[1].rotation.y = 0;
		else
			doom->renderables->values[1].rotation.y = M_PI_2;
		doom->renderables->values[1].dirty = TRUE;
	}

	if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_SEMICOLON))
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}

float move_speed = 1.5f;



void	hook_events(t_doom *doom)
{
	const double	ms = doom->stats.delta * 200.;
	const Uint8		*s = SDL_GetKeyboardState(NULL);
	SDL_Event		event;

	if (s[SDL_SCANCODE_ESCAPE])
		doom->running = FALSE;
	if (doom->current_gui == GUI_INGAME)
	{
		float dt = 1.0 / 60.;
		//long start = getMicrotime();
		entity_update(doom, &doom->player.entity, doom->stats.delta);
		//printf("delay %luus\n", getMicrotime() - start);
		move_speed = 50;
		if (s[SDL_SCANCODE_W] || s[SDL_SCANCODE_S])
		{
			doom->player.entity.velocity.x += sinf(doom->player.entity.rotation.y) * (s[SDL_SCANCODE_W] ? 1 : -1) * move_speed;
			doom->player.entity.velocity.z += cosf(doom->player.entity.rotation.y) * (s[SDL_SCANCODE_W] ? 1 : -1) * move_speed;
		}
		if (s[SDL_SCANCODE_A] || s[SDL_SCANCODE_D])
		{
			doom->player.entity.velocity.x += -cosf(doom->player.entity.rotation.y) * (s[SDL_SCANCODE_D] ? 1 : -1) * move_speed;
			doom->player.entity.velocity.z += sinf(doom->player.entity.rotation.y) * (s[SDL_SCANCODE_D] ? 1 : -1) * move_speed;
		} 
		if (s[SDL_SCANCODE_SPACE])
		{ 
			doom->player.entity.grounded = FALSE;
			doom->player.entity.velocity.y += 50;
		}
		if (s[SDL_SCANCODE_LSHIFT])
		{ 
			doom->player.entity.grounded = FALSE;
			doom->player.entity.velocity.y -= 50;
		}
		if (s[SDL_SCANCODE_J] || s[SDL_SCANCODE_L])
			doom->player.entity.rotation.y += 0.01 * (s[SDL_SCANCODE_J] ? 1 : -1) * ms;
		if (s[SDL_SCANCODE_I] || s[SDL_SCANCODE_K])
			doom->player.entity.rotation.x += 0.01 * (s[SDL_SCANCODE_I] ? 1 : -1) * ms;
		int m_x, m_y;
		SDL_GetRelativeMouseState(&m_x, &m_y);
		if (m_x != 0)
			doom->player.entity.rotation.y -= m_x * ms * 0.0001;
		if (m_y != 0)
		{
			float rot = m_y * ms * 0.0001;
			if (doom->player.entity.rotation.x - rot < M_PI_2 && doom->player.entity.rotation.x - rot > -M_PI_2 )
				doom->player.entity.rotation.x -= rot;
		}
	
		update_player_camera(&doom->player);
	}
	while (SDL_PollEvent(&event))
		events_window(doom, &event);
	SDL_PumpEvents();
}
