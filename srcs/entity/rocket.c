/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rocket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 10:24:25 by lloncham          #+#    #+#             */
/*   Updated: 2020/04/16 17:30:16 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "doom.h"
#include "render.h"
#include "sprite.h"
#include "entity.h"
#include "maths/mat4.h"
#include <math.h>

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
	rocket->damage = 0.5;
	rocket->range = 10;
	r->scale = (t_vec3){ 0.01, 0.01, 0.01 };
	r->of.data.entity->radius = (t_vec3){ 0.5, 0.5, 0.5 };
	r->of.data.entity->sources = rocket->sources;
	alGenSources(1, rocket->sources);
	compute_ellipsoid_hitbox(r, r->of.data.entity->position,
		r->of.data.entity->radius);
	return (TRUE);
}

t_bool	renderable_rocket(t_doom *doom, t_vec3 from, t_vec3 to)
{
	t_renderable rocket;

	copy_renderable(doom->res_manager.ressources->values[8]->data.model,
		&rocket);
	create_rocket(&rocket, doom);
	rocket.of.data.entity->position = from;
	rocket.of.data.entity->velocity = ft_vec3_mul_s(ft_vec3_norm(ft_vec3_sub(to,
		from)), 10 * doom->level.coeff_speed);
	rocket.of.data.entity->rotation = rotation_matrix_to_euler(look_at(to,
		from));
	append_renderables_array(&doom->renderables, rocket);
	return (TRUE);
}
