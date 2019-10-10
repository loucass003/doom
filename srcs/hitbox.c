/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hitbox.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 01:05:07 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/10 05:22:19 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "collision.h"
#include "render.h"
#include "entity.h"

void		compute_ellipsoid_hitbox(t_renderable *r, t_vec3 pos, t_vec3 radius)
{
	r->has_hitbox = TRUE;
	r->hitbox = (t_collidable) { 
		.type = COLLIDE_ELLIPSOID, 
		.data = { 
			.ellipsoid = (t_collide_ellipsoid){
				.origin = ft_vec3_add(r->hitbox_offset, pos),
				.radius = radius
			} 
		}
	};
}

void		update_hitbox(t_renderable *r)
{
	if (!r->has_hitbox)
		return ;
	if (r->entity && r->entity->type == ENTITY_ENEMY)
		compute_enemy_hitbox(r);
	else if (r->of.data.sprite)
		compute_sprite_hitbox(r);
}