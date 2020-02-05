/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 17:43:35 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/05 15:52:49 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "player.h"
#include "octree.h"

void				update_player_camera(t_player *player)
{
	t_camera		*camera;

	camera = &player->camera;
	camera->pos = player->entity.position;
	camera->rotation = player->entity.rotation;
	camera_update_maxtrix(&player->camera);	
	alListener3f(AL_POSITION, player->camera.pos.x, player->camera.pos.y, player->camera.pos.z);
	alListenerfv(AL_ORIENTATION, (ALfloat[6]){camera->forward.x, camera->forward.y, camera->forward.z, 0.f, 1.f, 0.f});
	// ALenum error = alGetError();
	// 	printf("alGenBuffers : %s\n", alGetString(error)); 
}

t_bool	create_player(t_renderable *r, t_doom *doom)
{
	if (!create_renderable(r, RENDERABLE_ENTITY))
		return (FALSE);
	r->of.data.entity = &doom->player.entity;
	r->of.data.entity->type = ENTITY_PLAYER;
	r->of.data.entity->packet.doom = doom;
	r->visible = FALSE;
	compute_ellipsoid_hitbox(r, r->of.data.entity->position, r->of.data.entity->radius);
	return (TRUE);
}

void				init_player(t_doom *doom)
{
	t_player		*player;

	player = &doom->player;
	player->camera.projection = projection_matrix();
	ft_bzero(player->item, sizeof(t_itemstack) * 8);
	ft_bzero(&doom->gameover, sizeof(t_gameover));
	// player->entity.position = (t_vec3){ 0, 2.512500, 45.176151 };
	player->entity.position = (t_vec3){ -30, 50, 0 };
	player->entity.velocity = (t_vec3){ 0, 0, 0 };
	player->entity.radius = (t_vec3){ 1, 2.5f, 1 };
	player->entity.packet.doom = doom;
	player->entity.rotation.y = -M_PI;
	player->entity.rotation.x = 0;
	player->entity.rotation.z = 0;
	player->entity.killable = TRUE;
	player->entity.life = doom->level.max_life;
	player->entity.max_life = doom->level.max_life;
	doom->main_context.camera = &player->camera;
	set_player_state(doom, player, PS_NORMAL);
	update_player_camera(&doom->player);
}

void				spawn_player(t_doom *doom)
{
	doom->player.entity.position = editor_to_world(doom->player.spawn_data.position);
	doom->player.entity.position.y += doom->player.entity.radius.y + 0.1;
	doom->player.entity.rotation = doom->player.spawn_data.rotation;
}

void				player_inventory_event(t_doom *doom, SDL_Event *event)
{
	const SDL_Scancode	key = event->key.keysym.scancode;

	if (doom->main_context.type == CTX_NORMAL)
	{
		if (event->type == SDL_MOUSEBUTTONDOWN)
		{
			t_itemstack	*is = &doom->player.item[doom->player.selected_slot];
			if (is->of && is->of->on_use)
				is->of->on_use(doom, is);
		}

		if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_UP || key == SDL_SCANCODE_DOWN))
		{
			doom->player.selected_slot += (key == SDL_SCANCODE_DOWN ? 1 : -1);
			if (doom->player.selected_slot < 0)
				doom->player.selected_slot = 7;
			if (doom->player.selected_slot > 7)
				doom->player.selected_slot = 0;
		}
		if (event->type == SDL_KEYDOWN && key == SDL_SCANCODE_C)
			set_player_state(doom, &doom->player, doom->player.desired_state == PS_CROUCH ? PS_NORMAL : PS_CROUCH);
	}
	if (doom->main_context.type == CTX_EDITOR)
	{
		if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_END))
		{
			doom->player.spawn_data.position = world_to_editor(doom->player.entity.position);
			doom->player.spawn_data.position.y -= doom->player.entity.radius.y;
			doom->player.spawn_data.rotation = doom->player.entity.rotation;
		}
	}
}

t_bool				draw_player_inventory(t_doom *doom, t_gui *self)
{
	static float	ticks = 0;

	if (doom->main_context.type == CTX_NORMAL)
	{
		int i = -1;
		while (++i < PLAYER_INV_SIZE)
		{
			t_itemstack		*is = &doom->player.item[i];
			if (i == doom->player.selected_slot)
				fill_rect(doom->main_context.image, (SDL_Rect){ 8, 48 + i * 60, 54, 54 }, 0xFFFFFF00);
			fill_rect(doom->main_context.image, (SDL_Rect){ 10, 50 + i * 60, 50, 50 }, 0xFFFF0000);
			if (is->of)
			{
				apply_image_blended(doom->main_context.image, is->of->image
					->data.texture, is->of->bounds,
					(SDL_Rect){ 10, 50 + i * 60, 50, 50 });
				if (is->amount <= 1)
					continue;
				const SDL_Color	color = {255, 255, 0, 0};
				SDL_Surface		*text;

				text = TTF_RenderText_Blended(doom->fonts.helvetica,
					ft_int_to_str(is->amount).str, color);
				apply_surface_blended(doom->main_context.image, text,
					(SDL_Rect){0, 0, text->w, text->h},
					(SDL_Rect){ 40, 50 + i * 60, 20, 20 });
				SDL_FreeSurface(text);
			}
		}

		ticks += doom->stats.delta * 30.;

		t_itemstack	*is = &doom->player.item[doom->player.selected_slot]; 

		if (is->of && is->of->type == ITEM_WEAPON)
		{
			t_weapon	*weapon = &is->of->data.weapon;
			if (ticks > 1 && weapon->fireing)
			{
				ticks = 0;
				weapon->current_step++;
				if (weapon->current_step == weapon->steps_count)
				{
					weapon->current_step = (SDL_GetMouseState(NULL, NULL) 
					& SDL_BUTTON(SDL_BUTTON_LEFT) ? 0 : weapon->idle_step);
					weapon->fireing = FALSE;
				}
				set_current_animation_step(weapon, weapon->animation_seq[weapon
					->current_step]);
			}
			if (weapon->type != WEAPON_GRENADA)
				apply_image_blended(doom->main_context.image, weapon->animation
				->data.texture, weapon->curr_image,
				(SDL_Rect){ S_WIDTH_2 - 80 / 2, S_HEIGHT - 300, 300, 300 });
		}
		((t_progress *)self->components->values[0])->value
		= doom->player.entity.life * (1 / doom->player.entity.max_life) * 100;
		if (doom->player.entity.life <= 0 || (doom->closer_boss
		&& doom->closer_boss->dead))
		{
			set_gui(doom, GUI_GAMEOVER);
			return (FALSE);
		}
	}
	return (TRUE);
}

void	update_controls(t_doom *doom)
{
	const double	ms = doom->stats.delta * 2.;
	const Uint8		*s = SDL_GetKeyboardState(NULL);

	float move_speed;
	if (!doom->mouse_focus && is_settings_open(&doom->editor))
		return ;
	if (doom->main_context.type == CTX_EDITOR)
		move_speed = 10;
	else if (doom->player.entity.jetpack)
		move_speed = 15;
	else if (s[SDL_SCANCODE_LSHIFT] && !doom->player.entity.jetpack)
		move_speed = 15;
	else
		move_speed = !doom->player.entity.grounded ? 1.2 : 10;
	if (s[SDL_SCANCODE_W] || s[SDL_SCANCODE_S])
	{
		doom->player.entity.velocity.x += sinf(doom->player.entity.rotation.y)
			* (s[SDL_SCANCODE_W] ? 1 : -1) * move_speed;
		doom->player.entity.velocity.z += cosf(doom->player.entity.rotation.y)
			* (s[SDL_SCANCODE_W] ? 1 : -1) * move_speed;
	}
	if (s[SDL_SCANCODE_A] || s[SDL_SCANCODE_D])
	{
		doom->player.entity.velocity.x += -cosf(doom->player.entity.rotation.y)
			* (s[SDL_SCANCODE_D] ? 1 : -1) * move_speed;
		doom->player.entity.velocity.z += sinf(doom->player.entity.rotation.y)
			* (s[SDL_SCANCODE_D] ? 1 : -1) * move_speed;
	}
	if (doom->main_context.type == CTX_NORMAL)
	{
		if (s[SDL_SCANCODE_SPACE] && !doom->player.entity.jump
		&& (doom->player.entity.grounded || doom->player.entity.jetpack))
			doom->player.entity.jump = TRUE;
		if (s[SDL_SCANCODE_LSHIFT] && doom->player.entity.jetpack)
		{ 
			doom->player.entity.grounded = FALSE;
			doom->player.entity.velocity.y -= 8;
		}
	
	}
	if (doom->main_context.type == CTX_EDITOR)
	{
		if (s[SDL_SCANCODE_SPACE] )
		{
			doom->player.entity.velocity.y += 8;
		}
		if (s[SDL_SCANCODE_LSHIFT])
		{ 
			doom->player.entity.velocity.y -= 8;
		}
	}
	if (doom->mouse_focus)
	{
		int m_x, m_y;
		SDL_GetRelativeMouseState(&m_x, &m_y);
		if (m_x != 0)
			doom->player.entity.rotation.y -= m_x * ms * 0.01;
		if (m_y != 0)
		{
			float rot = m_y * ms * 0.01;
			if (doom->player.entity.rotation.x - rot < M_PI_2
			&& doom->player.entity.rotation.x - rot > -M_PI_2 )
				doom->player.entity.rotation.x -= rot;
		}
	}
	update_player_camera(&doom->player);
}

t_bool	aabb_intersect_world(t_doom *doom, t_collide_aabb aabb)
{
	t_collision		hit;
	t_renderable	*r;
	int				i;
	int				j;

	i = -1;
	while (++i < doom->renderables->len)
	{
		r = &doom->renderables->values[i];
		if (r->of.type == RENDERABLE_ENTITY 
			&& (r->of.data.entity->dead
				|| (r->of.data.entity->type == ENTITY_ROCKET)
				|| (r->of.data.entity->type == ENTITY_PLAYER)))
			continue;
		if (r->has_hitbox && r->hitbox.type == COLLIDE_ELLIPSOID)
		{
			t_collide_ellipsoid ellipsoid = r->hitbox.data.ellipsoid;
			t_renderable *sphere = &doom->sphere_primitive;
			sphere->position = ellipsoid.origin;
			sphere->scale = ellipsoid.radius;
			sphere->dirty = TRUE;
			transform_renderable(sphere);
			r = sphere; 
		}
		j = -1;
		while (++j < r->faces->len)
		{
			if (!r->faces->values[j].has_collision && doom->main_context.type == CTX_NORMAL)
				continue;
			hit = triangle_hit_aabb(&r->faces->values[j].collidable.data.triangle, &aabb);
			if (hit.collide)
			{
				printf("HIT %d\n", r->of.type);
				return (TRUE);
			}
		}
	}
	return (FALSE);
}


t_bool		set_player_height(t_doom *doom, t_player *player, float height)
{
	t_entity	*e;
	float		diff;

	e = &player->entity;

	diff = (height / 2.0) - e->radius.y;
	// printf("%f diff\n", diff);
	//e->velocity.y = 5;
	if (diff > 0)
	{
		t_vec3 radius = (t_vec3){ e->radius.x, height / 2, e->radius.z };
		t_collide_aabb aabb = (t_collide_aabb){ 
			.min = ft_vec3_sub(e->position, (t_vec3){ e->radius.x, 0, e->radius.z }),
			.max = ft_vec3_add(e->position, radius) 
		};
		if (aabb_intersect_world(doom, aabb))
			return (FALSE);
	}
	// e->velocity.y = 500;
	e->radius.y = height / 2;
	if (diff < 0)
		e->position.y -= height / 2;
	if (diff > 0)
		e->position.y += height / 2;
	return (TRUE);
}

void		set_player_state(t_doom *doom, t_player *player, t_player_state state)
{
	player->desired_state = state;
	if (state == PS_CROUCH)
	{
		if (set_player_height(doom, player, 2.5))
			player->player_state = state;
	}
	else if (state == PS_NORMAL)
	{
		if (set_player_height(doom, player, 3 * 2))
			player->player_state = state;
	}
}
