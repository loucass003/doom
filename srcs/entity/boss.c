/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boss.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/28 14:15:25 by lloncham          #+#    #+#             */
/*   Updated: 2019/12/01 00:57:36 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "doom.h"
#include "render.h"
#include "sprite.h"
#include "entity.h"
#include <stdlib.h>


t_bool		create_boss_renderable(t_doom *doom, t_renderable *r)
{
	t_sprite	*sprite;
	t_entity	*boss;

	if (!(sprite = create_sprite((t_vec2){ 7, 4 }, doom->res_manager.ressources->values[6])))
		return (FALSE);
	if (!create_sprite_renderable(r, sprite))
		return (FALSE);
	set_current_cell(r, 0, 0);
	r->of.type = RENDERABLE_ENTITY;
	if (!(boss = ft_memalloc(sizeof(t_entity))))
		return (FALSE);
	boss->type = ENTITY_BOSS;
	boss->packet.doom = doom;
	boss->scale = (t_vec3){ 5, 5, 5 };
	boss->radius = (t_vec3){ 2.5, 2.5, 2.5 };
	boss->life = 8;
	boss->max_life = 8;
	boss->sources = boss->of.boss.sources;
	alGenSources(3, boss->sources);
	r->of.data.entity = boss;
	r->scale = boss->scale;
	compute_entity_hitbox(r);
	return (TRUE);
}

void		entity_update_boss(t_doom *doom, t_entity *entity, double dt)
{
	t_entity_boss	*boss;

	boss = &entity->of.boss;
	if (boss->dead)
		return ;
	if (boss->phase == 0 && entity->life < entity->max_life / 2)
	{
		boss->animation_step = 0;
		boss->phase = 3;
	}
	if (boss->phase == 1 && entity->life <= 0)
	{
		boss->animation_step = 0;
		boss->phase = 2;
	}
	t_vec3 dir = ft_vec3_sub(doom->player.entity.position, entity->position);
 	t_vec3 norm_dir = ft_vec3_norm(dir);
	t_ray ray = (t_ray){ .origin = doom->player.entity.position, .direction = ft_vec3_inv(norm_dir) };
	t_vec3 view = ft_vec3_sub(doom->player.entity.position, entity->position);
	view = vec3_rotate(view, (t_vec3){ 0, -entity->rotation.y, 0 });
	float a = atan2(view.z, view.x);
	boss->hit_data.collide = FALSE;
	boss->hit_data = ray_hit_world(doom, doom->renderables, ray);
	if (boss->hit_data.collide && boss->hit_data.renderable && boss->hit_data.renderable->of.data.entity == entity && boss->hit_data.dist < 50)
		boss->focus = TRUE;
	else
	{
		boss->focus = FALSE;
		boss->animation_step = 0;
	}
	entity->rotation.y = doom->player.camera.rotation.y + M_PI_2;
	if (boss->hit_data.dist > 20 && boss->phase <= 1)
		entity->velocity = ft_vec3_add(entity->velocity, ft_vec3_mul_s(norm_dir, 5));
	boss->t0 += 5 * dt;
	if (boss->focus && boss->t0 > 1)
	{
		boss->t0 = 0;
		if (boss->phase <= 1)
		{
			t_bool walking = ft_vec3_dot(entity->velocity, entity->velocity) != 0;
			if (boss->hit_data.dist > 20 && walking)
			{
				boss->animation_step++;
				if (boss->animation_step >= 4)
					boss->animation_step = 0;
			}
			if (boss->hit_data.dist <= 20)
			{
				if (boss->shooting)
					boss->t1 = 0;
				boss->t1++;
				if (boss->t1 >= 5 && !boss->shooting)
				{
					boss->shooting = TRUE;
					boss->t1 = 0;
					boss->animation_step = 4;
				}

				if (boss->shooting)
				{
					boss->animation_step++;
					if (boss->animation_step >= 7)
					{
						boss->animation_step = 4;
						boss->shooting = FALSE;
					}
				}
				else
					boss->animation_step = 4;
			}
		}
		if (boss->phase == 3)
		{
			boss->animation_step++;
			if (boss->animation_step >= 3)
			{
				boss->phase = 1;
				boss->animation_step = 4;
				t_sprite *sprite = create_sprite((t_vec2){ 7, 4 }, doom->res_manager.ressources->values[6]);
				t_renderable r;
				create_sprite_renderable(&r, sprite);
				r.position = entity->position;
				r.scale = (t_vec3){ 5, 5, 5 };
				r.no_collision = TRUE;
				set_current_cell(&r, 3, 3);
				append_renderables_array(&doom->renderables, r);
			}
		}
		if (boss->phase == 2)
		{
			boss->animation_step++;
			if (boss->animation_step >= 7)
			{
				boss->dead = TRUE;
				boss->animation_step = 6;
			}
		}
	}
}