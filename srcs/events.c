/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 22:14:55 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/21 14:46:33 by llelievr         ###   ########.fr       */
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
#include "gui.h"

// void		component_to_union(t_components_u *u, t_component *c)
// {
// 	if (c->type == C_SELECT)
// 		u->select = *(t_select *)c;
// 	else if (c->type == C_MENU)
// 		u->menu = *(t_menu *)c;
// 	else if (c->type == C_PROGRESS)
// 		u->progress = *(t_progress *)c;
// 	else if (c->type == C_BUTTON)
// 		u->button = *(t_button *)c;
// }

// void		union_to_component(t_components_u *u, t_component_type type, t_component *c)
// {
// 	if (type == C_SELECT)
// 		*((t_select *)c) = u->select;
// 	else if (type == C_MENU)
// 		*((t_menu *)c) = u->menu;
// 	else if (type == C_PROGRESS)
// 		*((t_progress *)c) = u->progress;
// 	else if (type == C_BUTTON)
// 		*((t_button *)c) = u->button;
// }

static void	events_window(t_doom *doom, SDL_Event *event)
{
	const SDL_Scancode	key = event->key.keysym.scancode;

	gui_events(doom, doom->guis, event, doom->current_gui);
	components_events(doom, doom->guis, event, doom->current_gui);
	components_events(doom, doom->guis, event, GUI_EDITOR_SETTINGS);
	if (event->type == SDL_QUIT)
		doom->running = FALSE;
	if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		t_itemstack	*is = &doom->player.item[doom->player.selected_slot];
		if (is->of && is->of->on_use)
			is->of->on_use(doom, is);
	}
	// if (event->type == SDL_MOUSEBUTTONUP && doom->current_gui >= 0)
	// {
	// 	for (int i = 0; i < doom->guis[doom->current_gui].components->len; i++)
	// 		if (doom->guis[doom->current_gui].components->values[i]->on_click)
	// 			doom->guis[doom->current_gui].components->values[i]
	// 				->on_click(doom->guis[doom->current_gui].components->values[i],
	// 				(t_vec2){ event->button.x, event->button.y }, doom);
	// }
	// if (event->type == SDL_MOUSEMOTION && doom->current_gui >= 0)
	// {
	// 	doom->mouse = (t_vec2){ event->motion.x, event->motion.y };
	// 	for (int i = 0; i < doom->guis[doom->current_gui].components->len; i++)
	// 	{
	// 		if (doom->guis[doom->current_gui].components->values[i]->on_mouse_move)
	// 			doom->guis[doom->current_gui].components->values[i]
	// 				->on_mouse_move(doom->guis[doom->current_gui].components->values[i],
	// 				doom->mouse, doom);
	// 	}
	// }
	if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_P))
	{
		t_ray ray = create_shoot_ray(doom->player, (t_vec3){0, 0, 1});
		t_collision hit = ray_hit_world(doom, doom->renderables, ray);
		if (hit.collide)
		{
			t_renderable *r = hit.renderable;
			if (r && r->of.type != RENDERABLE_ENTITY)
			{
				t_renderable enemy;
				create_enemy(doom, &enemy);
				enemy.of.data.entity->position = hit.point;
				enemy.of.data.entity->position.y += enemy.of.data.entity->radius.y;
				append_renderables_array(&doom->renderables, enemy);
			}
		}
	}

	if (event->type == SDL_KEYDOWN && key == SDL_SCANCODE_G)
	{
		t_renderable grenada;
		grenada = doom->grenada_primitive;
		create_grenada(&grenada, doom);
		grenada.of.data.entity->position = doom->player.entity.position;
		t_vec3 forward = vec3_rotate((t_vec3){ 0, 0, 1 }, (t_vec3){-doom->player.entity.rotation.x, doom->player.entity.rotation.y, 0});
		printf("%f %f %f\n", forward.x, forward.y, forward.z);
		forward.y *= 20;
		forward.x *= 14;
		forward.z *= 14;
		grenada.of.data.entity->velocity = forward;
		append_renderables_array(&doom->renderables, grenada);
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

	if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_UP || key == SDL_SCANCODE_DOWN))
	{
		doom->player.selected_slot += (key == SDL_SCANCODE_DOWN ? 1 : -1);
		if (doom->player.selected_slot < 0)
			doom->player.selected_slot = 7;
		if (doom->player.selected_slot > 7)
			doom->player.selected_slot = 0;
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
	if (s[SDL_SCANCODE_HOME])
	{
		set_gui(doom, GUI_MAIN_MENU);
	}
	if (doom->current_gui == GUI_INGAME)
	{
		float dt = 1.0 / 60.;
		//long start = getMicrotime();
		entity_update(doom, &doom->player.entity, doom->stats.delta);
		//printf("delay %luus\n", getMicrotime() - start);
		move_speed = 25;
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
