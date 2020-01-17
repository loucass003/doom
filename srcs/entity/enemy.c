/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/30 16:36:08 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/17 14:37:43 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "doom.h"
#include "render.h"
#include "sprite.h"
#include "entity.h"
#include <stdlib.h>

t_bool		create_enemy_renderable(t_doom *doom, t_renderable *r)
{
	t_sprite	*sprite;
	t_entity	*enemy;

	if (!(sprite = create_sprite((t_vec2){ 8, 7 }, doom->res_manager.ressources->values[5])))
		return (FALSE);
	if (!create_sprite_renderable(r, sprite))
		return (FALSE);
	set_current_cell(r, 0, 0);
	r->of.type = RENDERABLE_ENTITY;
	if (!(enemy = ft_memalloc(sizeof(t_entity))))
		return (FALSE);
	enemy->type = ENTITY_ENEMY;
	enemy->packet.doom = doom;
	enemy->scale = (t_vec3){ 5, 5, 5 };
	enemy->radius = (t_vec3){ 1, 2.5, 1 };
	enemy->life = 1;
	enemy->sources = enemy->of.enemy.sources;
	alGenSources(3, enemy->sources);
	r->of.data.entity = enemy;
	r->scale = enemy->scale;
	compute_entity_hitbox(r);
	return (TRUE);
}

void		entity_update_enemy(t_doom *doom, t_entity *entity, double dt)
{
	t_entity_enemy	*enemy;
	t_bool			walking;


	enemy = &entity->of.enemy;
	if (enemy->died)
		return ;
	if (entity->life <= 0 && !enemy->diying)
	{
		enemy->diying = TRUE;
		enemy->animation_step = 5;
		// play_music(&doom->audio, entity->position, 5, FALSE);
		entity_sound(entity, 0, 0, 2);
	}
	enemy->t0 += 5 * dt;
	t_vec3 dir = ft_vec3_sub(doom->player.entity.position, entity->position);
 	t_vec3 norm_dir = ft_vec3_norm(dir);
	t_ray ray = (t_ray){ .origin = doom->player.entity.position, .direction = ft_vec3_inv(norm_dir) };
	t_vec3 view = ft_vec3_sub(doom->player.entity.position, entity->position);
	view = vec3_rotate(view, (t_vec3){ 0, -entity->rotation.y, 0 });
	float a = atan2(view.z, view.x);
	enemy->hit_data.collide = FALSE;

	if ((a < 1.5 && a > -1.5) || enemy->focus)
		enemy->hit_data = ray_hit_world(doom, doom->renderables, ray);
	if (enemy->hit_data.collide && enemy->hit_data.renderable && enemy->hit_data.renderable->of.data.entity == entity && enemy->hit_data.dist < 50)
		enemy->focus = TRUE;
	else
	{
		enemy->focus = FALSE;
		enemy->animation_step = 0;
	}

	if (enemy->focus || enemy->diying)
	{
		entity->rotation.y = doom->player.camera.rotation.y + M_PI_2;
		if (!enemy->diying && enemy->hit_data.dist > 20)
			entity->velocity = ft_vec3_add(entity->velocity, ft_vec3_mul_s(norm_dir, 10 * doom->level.coeff_speed));
	}
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
		if (enemy->focus)
		{
			if (!walking)
				enemy->animation_step = 6;
			enemy->t1++;
			if (enemy->t1 >= 5 && !walking && !enemy->diying)
			{
				enemy->shooting = TRUE;
				enemy->t1 = 0;

				entity_sound(entity, 1, 1, 1);
				uint8_t u = rand();
				if (u > 120)
					doom->player.entity.life -= 0.1;
			}
		}
		if (enemy->diying)
		{
			enemy->animation_step = 5;
			if (++enemy->diying_step == 4){
				enemy->died = TRUE;
				
			}
				
		}
	}
	//entity->velocity.y = 15;
}
