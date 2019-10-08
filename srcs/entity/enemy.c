/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/30 16:36:08 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/08 19:53:20 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "doom.h"
#include "render.h"
#include "sprite.h"
#include "entity.h"

// t_collision		ray_hit_entity(t_renderables *renderables, t_ray *ray, t_entity *collided)
// {
// 	t_collision	min;
// 	t_collision	hit;
// 	int			i;
// 	float		dist;

// 	min = (t_collision) { .collide = FALSE, .dist = -1.0 };
// 	dist = INT_MAX;
// 	i = -1;
// 	while (++i < renderables->len)
// 	{
// 		t_renderable *r = &renderables->values[i];
// 		if (!r->entity)
// 			continue;
// 		hit = ray_hit_aabb(ray, &(t_collide_aabb){
// 			.min = ft_vec3_sub(r->entity->position, r->entity->radius),
// 			.max = ft_vec3_add(r->entity->position, r->entity->radius)
// 		});
// 		if (hit.collide && hit.dist < dist)
// 		{
// 			dist = hit.dist;
// 			collided = r->entity;
// 			min = hit;
// 		}
// 	}
// 	return (min);
// }

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
