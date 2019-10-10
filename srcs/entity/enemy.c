/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/30 16:36:08 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/10 23:29:25 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "doom.h"
#include "render.h"
#include "sprite.h"
#include "entity.h"

void		compute_enemy_hitbox(t_renderable *r)
{
	compute_ellipsoid_hitbox(r, r->entity->position, r->entity->radius);
}

t_bool		create_enemy(t_doom *doom, t_renderable *r)
{
	if (!create_sprite(r, (t_mtl){ .texture_map = doom->textures.sprite, .texture_map_set = TRUE }, (t_vec2){ 8, 7 }))
		return (FALSE);
	set_current_cell(r, 0, 0);
	r->scale = (t_vec3){ 5, 5, 5 };
	r->entity = ft_memalloc(sizeof(t_entity));
	r->entity->type = ENTITY_ENEMY;
	r->entity->packet.doom = doom;
	r->entity->radius = (t_vec3){ 1, 2.5, 1 };
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
	t_vec3 e_dir = vec3_rotate((t_vec3){0, 0, -1}, entity->rotation);
	float f = acosf(ft_vec3_dot(norm_dir, e_dir));
	float dist = ft_vec3_len(dir);
	if (f < 1.59 && dist < 50)
		entity->focus = TRUE;
	else if (dist > 50)
		entity->focus = FALSE;
	if (entity->focus)
	{
		entity->rotation.y = doom->player.camera.rotation.y + M_PI_2;
		if (dist > 15)
			entity->velocity = ft_vec3_add(entity->velocity, ft_vec3_mul_s(norm_dir, 10));
	}
	//entity->velocity = ft_vec3_mul_s(entity->velocity, 0.8);
	if (entity->t0 > 1)
	{
		entity->t0 = 0;
		walking = ft_vec3_dot(entity->velocity, entity->velocity) != 0;
		if (walking)
		{
			entity->animation_step++;
			if (entity->animation_step == 5)
				entity->animation_step = walking;
		}
		else
			entity->animation_step = 0;
	}
	entity->velocity.y = 15;
}
