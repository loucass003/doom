/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 15:28:44 by lloncham          #+#    #+#             */
/*   Updated: 2020/02/19 15:40:01 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "doom.h"
#include "render.h"
#include "sprite.h"
#include "entity.h"
#include <stdlib.h>

void	entity_update_sound(t_entity *entity)
{
	ALint status;

	if ((entity->velocity.x || entity->velocity.z) && entity->grounded)
	{
		alGetSourcei(entity->sources[2], AL_SOURCE_STATE, &status);
		if (status != AL_PLAYING)
			entity_sound(entity, 2, 2, 1);
	}
}

void	enemy_focus_step(t_doom *doom, t_entity *entity, t_entity_enemy *enemy,
	t_bool walking)
{
	uint8_t u;

	if (enemy->focus)
	{
		if (!walking)
			enemy->animation_step = 6;
		enemy->t1++;
		if (enemy->t1 >= 5 && !walking && !entity->diying)
		{
			enemy->shooting = TRUE;
			enemy->t1 = 0;
			entity_sound(entity, 1, 1, 1);
			u = rand();
			if (u > 120)
				doom->player.entity.life -= 0.1;
		}
	}
}

void	update_entity_state(t_doom *doom, t_entity *entity,
	t_entity_enemy *enemy)
{
	t_bool	walking;

	if (enemy->t0 > 1)
	{
		enemy->t0 = 0;
		enemy->shooting = FALSE;
		walking = ft_vec3_dot(entity->velocity, entity->velocity) != 0;
		if (enemy->hit_data.dist > 20)
		{
			enemy->animation_step++;
			if (enemy->animation_step >= 5)
				enemy->animation_step = 1;
		}
		if (!walking)
			enemy->animation_step = 0;
		enemy_focus_step(doom, entity, enemy, walking);
		if (entity->diying)
		{
			enemy->animation_step = 5;
			if (++enemy->diying_step == 4)
				entity->dead = TRUE;
		}
	}
}

void	set_enemy_focus(t_doom *doom, t_entity *entity, t_entity_enemy *enemy,
	t_vec3 norm_dir)
{
	if (enemy->hit_data.collide && enemy->hit_data.renderable
		&& enemy->hit_data.renderable->of.data.entity == entity
		&& enemy->hit_data.dist < 50)
		enemy->focus = TRUE;
	else
	{
		enemy->focus = FALSE;
		enemy->animation_step = 0;
	}
	if (enemy->focus || entity->diying)
	{
		entity->rotation.y = doom->player.camera.rotation.y + M_PI_2;
		if (!entity->diying && enemy->hit_data.dist > 20)
			entity->velocity = ft_vec3_add(entity->velocity,
				ft_vec3_mul_s(norm_dir, 10 * doom->level.coeff_speed));
	}
}

void	entity_anim_step(t_doom *doom, t_entity *entity, t_entity_enemy *enemy)
{
	t_vec3	dir;
	t_vec3	norm_dir;
	t_ray	ray;
	t_vec3	view;
	float	a;

	dir = ft_vec3_sub(doom->player.entity.position, entity->position);
	norm_dir = ft_vec3_norm(dir);
	ray = (t_ray){
		.origin = doom->player.entity.position,
		.direction = ft_vec3_inv(norm_dir) };
	view = ft_vec3_sub(doom->player.entity.position, entity->position);
	view = vec3_rotate(view, (t_vec3){ 0, -entity->rotation.y, 0 });
	a = atan2(view.z, view.x);
	enemy->hit_data.collide = FALSE;
	if ((a < 1.5 && a > -1.5) || enemy->focus)
		enemy->hit_data = ray_hit_world(doom, doom->renderables, ray);
	set_enemy_focus(doom, entity, enemy, norm_dir);
	update_entity_state(doom, entity, enemy);
}
