/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boss_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 17:25:01 by lloncham          #+#    #+#             */
/*   Updated: 2020/02/19 17:27:13 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "doom.h"
#include "render.h"
#include "sprite.h"
#include "entity.h"
#include <math.h>
#include <stdlib.h>

void		set_boss_focus(t_entity *entity, t_entity_boss *boss)
{
	if (boss->hit_data.collide && boss->hit_data.renderable
		&& boss->hit_data.renderable->of.data.entity == entity
		&& boss->hit_data.dist < 50)
		boss->focus = TRUE;
	else
	{
		boss->focus = FALSE;
		boss->animation_step = 0;
	}
}

void		boss_rocket(t_doom *doom, t_entity *entity, t_entity_boss *boss)
{
	t_sprite		*sprite;
	t_renderable	r;

	boss->phase = 1;
	boss->animation_step = 4;
	sprite = create_sprite((t_vec2){ 7, 4 },
		doom->res_manager.ressources->values[6]);
	create_sprite_renderable(&r, sprite);
	r.position = entity->position;
	r.scale = (t_vec3){ 5, 5, 5 };
	r.no_collision = TRUE;
	set_current_cell(&r, 3, 3);
	append_renderables_array(&doom->renderables, r);
}

void		boss_hit_dist_step(t_doom *doom, t_entity *entity,
	t_entity_boss *boss)
{
	t_bool walking;

	walking = ft_vec3_dot(entity->velocity, entity->velocity) != 0;
	if (boss->hit_data.dist >= 20 && walking)
	{
		boss->animation_step++;
		if (boss->animation_step >= 4)
			boss->animation_step = 0;
	}
	if (boss->hit_data.dist <= 20)
		boss_hit_dist(doom, entity, boss);
}

void		boss_anim_state(t_doom *doom, t_entity *entity, t_entity_boss *boss)
{
	if (boss->focus && boss->t0 > 1)
	{
		boss->t0 = 0;
		if (boss->phase <= 1)
			boss_hit_dist_step(doom, entity, boss);
		if (boss->phase == 3)
		{
			boss->animation_step++;
			if (boss->animation_step >= 3)
				boss_rocket(doom, entity, boss);
		}
		if (boss->phase == 2)
		{
			entity->diying = TRUE;
			boss->animation_step++;
			if (boss->animation_step >= 7)
			{
				entity->dead = TRUE;
				boss->animation_step = 6;
			}
		}
	}
}

void		boss_update_step(t_doom *doom, t_entity *entity,
	t_entity_boss *boss, double dt)
{
	t_vec3	dir;
	t_vec3	norm_dir;
	t_ray	ray;
	t_vec3	view;

	dir = ft_vec3_sub(doom->player.entity.position, entity->position);
	norm_dir = ft_vec3_norm(dir);
	ray = (t_ray){
		.origin = doom->player.entity.position,
		.direction = ft_vec3_inv(norm_dir) };
	view = ft_vec3_sub(doom->player.entity.position, entity->position);
	view = vec3_rotate(view, (t_vec3){ 0, -entity->rotation.y, 0 });
	boss->hit_data.collide = FALSE;
	boss->hit_data = ray_hit_world(doom, doom->renderables, ray);
	entity->rotation.y = doom->player.camera.rotation.y + M_PI_2;
	if (boss->hit_data.dist > 20 && boss->phase <= 1)
		entity->velocity = ft_vec3_add(entity->velocity, ft_vec3_mul_s(norm_dir,
			4 * doom->level.coeff_speed));
	boss->t0 += 5 * dt;
	set_boss_focus(entity, boss);
	boss_anim_state(doom, entity, boss);
}
