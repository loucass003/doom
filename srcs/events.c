/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 22:14:55 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/06 03:29:39 by llelievr         ###   ########.fr       */
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
			if (r)
			{
				//splice_faces_array(r->faces, min.who.data.triangle.face, 1);
				// t_face *face = &r->faces->values[min.who.data.triangle.face];
				// face->hidden = TRUE;
				t_renderable sprite;
				create_sprite(&sprite, (t_mtl){ .texture_map = doom->textures.sprite, .texture_map_set = TRUE }, (t_vec2){ 8, 7 });
				set_current_cell(&sprite, 0, 0);
				sprite.scale = (t_vec3){ 5, 6, 5 };
				//sprite.of.data.sprite->always_facing_player = FALSE;
				sprite.entity = ft_memalloc(sizeof(t_entity));
				sprite.entity->position = min.point;
				sprite.entity->position.y += 2.5;
				sprite.entity->type = ENTITY_ENEMY;
				sprite.entity->packet.doom = doom;
				sprite.entity->radius = (t_vec3){ 1, 2.5f, 1 };
				sprite.entity->pos_offset.y = -2.5;
				// sprite.entity->velocity.x += 4;
				append_renderables_array(&doom->renderables, sprite);
			}
		}
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

float move_speed = 1.5f;

#include <sys/time.h>
long getMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

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
		long start = getMicrotime();
		entity_update(doom, &doom->player.entity, dt);
		printf("delay %luus\n", getMicrotime() - start);
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
			doom->player.entity.velocity.y += 2.1;
		}
		if (s[SDL_SCANCODE_J] || s[SDL_SCANCODE_L])
			doom->player.entity.rotation.y += 0.01 * (s[SDL_SCANCODE_J] ? 1 : -1) * ms;
		if (s[SDL_SCANCODE_I] || s[SDL_SCANCODE_K])
			doom->player.entity.rotation.x += 0.01 * (s[SDL_SCANCODE_I] ? 1 : -1) * ms;
		int m_x, m_y;
		SDL_GetRelativeMouseState(&m_x, &m_y);
		if (m_x != 0)
		{
			doom->player.entity.rotation.y -= m_x * ms * 0.00005;
			// wolf->player.matrix = ft_mat2_rotation(wolf->player.rotation - M_PI_2);
		}
		if (m_y != 0)
		{
			float rot = m_y * ms * 0.00005;
			if (doom->player.entity.rotation.x - rot < M_PI_2 && doom->player.entity.rotation.x - rot > -M_PI_2 )
				doom->player.entity.rotation.x -= rot;
		}
		move_speed = 1.0f;
	
	//	doom->player.entity.velocity.z = 200 * dt;
	//	doom->player.entity.velocity.y -= 100 * dt;
		//update_player_camera(&doom->player);
		
		
		update_player_camera(&doom->player);
		 //		doom->player.entity.velocity.y -= (100.0f *  doom->stats.delta);
		//	doom->player.entity.position.y += 1.0f;
	/* 	printf("%f %f %f - %f %f %f\n", 
			doom->player.entity.position.x,
			doom->player.entity.position.y,
			doom->player.entity.position.z,
			doom->player.entity.velocity.x,
			doom->player.entity.velocity.y,
			doom->player.entity.velocity.z); */
	}
	// // test_collision(doom);
	// // doom->player.pos = ft_vec3_add(doom->player.pos, ft_vec3_mul_s(doom->player.velocity, ms));
	// // doom->player.velocity.y -= 0.05;
	// // if (doom->current_gui == GUI_INGAME)
	// // {
	// 	t_physics_data packet;
	// 	ft_bzero(&packet, sizeof(t_physics_data));
	// 	packet.distance = INT_MAX;
	// 	packet.e_radius = (t_vec3){ 0.25, 0.25, 0.25 };
	// 	packet.doom = doom;
	// 	packet.r3_posision = doom->player.pos;
	// 	packet.r3_velocity = doom->player.velocity;
	// 	packet.e_position = ft_vec3_div(packet.r3_posision, packet.e_radius);
	// 	packet.e_velocity = ft_vec3_div(packet.r3_velocity, packet.e_radius);
	// 	collide_and_slide(&packet, (t_vec3){ 0, doom->player.velocity.y, 0 });
	// 	doom->player.pos = packet.r3_posision;
	// //	doom->player.velocity = packet.r3_velocity;
	// 	doom->player.velocity = ft_vec3_mul_s(doom->player.velocity, 0.6);
	// 	update_player_camera(&doom->player);
	// // }
	
	while (SDL_PollEvent(&event))
		events_window(doom, &event);
	SDL_PumpEvents();
}
