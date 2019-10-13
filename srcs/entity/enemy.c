/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/30 16:36:08 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/13 19:12:18 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "doom.h"
#include "render.h"
#include "sprite.h"
#include "entity.h"
#include <stdlib.h>

void		compute_enemy_hitbox(t_renderable *r)
{
	const t_entity	*entity = r->of.data.entity;

	compute_ellipsoid_hitbox(r, entity->position, entity->radius);
}

t_bool		create_enemy(t_doom *doom, t_renderable *r)
{
	if (!create_sprite(r, (t_mtl){ .texture_map = doom->textures.sprite, .texture_map_set = TRUE }, (t_vec2){ 8, 7 }))
		return (FALSE);
	set_current_cell(r, 0, 0);
	r->scale = (t_vec3){ 5, 5, 5 };
	r->of.type = RENDERABLE_ENTITY;
	if (!(r->of.data.entity = ft_memalloc(sizeof(t_entity))))
		return (FALSE);
	r->of.data.entity->type = ENTITY_ENEMY;
	r->of.data.entity->packet.doom = doom;
	r->of.data.entity->radius = (t_vec3){ 1, 2.5, 1 };
	//r->show_hitbox = TRUE;
	//r->entity->pos_offset.y = -r->entity->radius.y;
	compute_enemy_hitbox(r);
	return (TRUE);
}

void		entity_update_enemy(t_doom *doom, t_entity *entity, double dt)
{
	t_bool			walking;

	entity->t0 += 5 * dt;
	t_vec3 dir = ft_vec3_sub(doom->player.entity.position, entity->position);
 	t_vec3 norm_dir = ft_vec3_norm(dir);
	t_ray ray = (t_ray){ .origin = doom->player.entity.position, .direction = ft_vec3_inv(norm_dir) };
	t_vec3 view = ft_vec3_sub(doom->player.entity.position, entity->position);
	view = vec3_rotate(view, (t_vec3){ 0, -entity->rotation.y, 0 });
	float a = atan2(view.z, view.x);
	entity->hit_data.collide = FALSE;
	
	if ((a < 1.5 && a > -1.5) || entity->focus)
		entity->hit_data = ray_hit_world(doom, doom->renderables, ray);
	if (entity->hit_data.collide && entity->hit_data.renderable && entity->hit_data.renderable->of.data.entity == entity && entity->hit_data.dist < 50)
		entity->focus = TRUE;
	else
	{
		entity->focus = FALSE;
		entity->animation_step = 0;
	}
	if (entity->focus)
	{
		entity->rotation.y = doom->player.camera.rotation.y + M_PI_2;
		if (entity->hit_data.dist > 20)
			entity->velocity = ft_vec3_add(entity->velocity, ft_vec3_mul_s(norm_dir, 10));
		else
			entity->animation_step = 6;
	}

	//entity->velocity = ft_vec3_mul_s(entity->velocity, 0.8);
	if (entity->t0 > 1)
	{
		entity->shooting = FALSE;
		entity->t0 = 0;
		walking = ft_vec3_dot(entity->velocity, entity->velocity) != 0;
		if (entity->focus)
		{
			entity->t1++;
			if (entity->t1 >= 5 && !walking)
			{
				entity->shooting = TRUE;
				entity->t1 = 0;
				uint8_t u = rand();
				if (u > 100)
					doom->player.entity.life -= 0.1;
			}
		}
		
		if (walking && entity->hit_data.dist > 20)
		{
			entity->animation_step++;
			if (entity->animation_step == 5)
				entity->animation_step = walking;
		}
	}
	entity->velocity.y = 15;
}
