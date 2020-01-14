/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rocket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 10:24:25 by lloncham          #+#    #+#             */
/*   Updated: 2020/01/14 16:27:04 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "doom.h"
#include "render.h"
#include "sprite.h"
#include "entity.h"
#include <math.h>
#include <stdlib.h>

t_bool	create_rocket(t_renderable *r, t_doom *doom)
{
	t_entity_rocket	*rocket;
	
	r->of.type = RENDERABLE_ENTITY;
	if (!(r->of.data.entity = ft_memalloc(sizeof(t_entity))))
		return (FALSE);
	r->of.data.entity->type = ENTITY_ROCKET;
	r->of.data.entity->packet.doom = doom;
	
	r->visible = TRUE;
	rocket = &r->of.data.entity->of.rocket;
	rocket->damage = 5;
	rocket->range = 10;
	r->scale = (t_vec3){ 0.01, 0.01, 0.01 };
	r->of.data.entity->radius = (t_vec3){ 0.5, 0.5, 0.5 };
	//r->show_hitbox = TRUE;
	compute_ellipsoid_hitbox(r, r->of.data.entity->position, r->of.data.entity->radius);
	return (TRUE);
}

t_vec3	rotation_matrix_to_euler(t_mat4 m)
{
	float sy = sqrtf(m.a[0][0] * m.a[0][0] + m.a[0][1] * m.a[0][1]);
	t_bool singlular = sy < 1e-6;
	t_vec3 out;

	if (!singlular)
	{
		out.x = atan2(m.a[1][2], m.a[2][2]);
		out.y = atan2(-m.a[0][2], sy);
		out.z = atan2(m.a[0][1], m.a[0][0]);
	}
	else
	{
		out.x = atan2(-m.a[2][1], m.a[1][1]);
		out.y = atan2(-m.a[0][2], sy);
		out.z = 0;
	}
	return (out);	
}

t_bool	renderable_rocket(t_doom *doom, t_vec3 from, t_vec3 to)
{
	t_renderable rocket;
	
	rocket = *doom->res_manager.ressources->values[8]->data.model;
	create_rocket(&rocket, doom);
	rocket.of.data.entity->position = from;
	rocket.of.data.entity->velocity = ft_vec3_mul_s(ft_vec3_norm(ft_vec3_sub(to, from)), 8);
	rocket.of.data.entity->rotation = rotation_matrix_to_euler(look_at(to, from));
	append_renderables_array(&doom->renderables, rocket);
	return (TRUE);
}

// t_bool  entity_hit_rocket(t_entity *entity, t_renderable r)
// {
    
// }