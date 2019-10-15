/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 00:01:14 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/15 18:57:28 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENTITY_H
# define ENTITY_H

# include "libft.h"
# include "collision.h"

typedef enum		e_entity_type
{
	ENTITY_PLAYER,
	ENTITY_ENEMY
}					t_entity_type;

typedef struct		s_entity
{
	t_entity_type	type;
	t_vec3			position;
	t_vec3			velocity;
	t_vec3			rotation;
	t_vec3			radius;
	t_bool			grounded;
	t_physics_data	packet;
	float			life;
	float			max_life;
	float			t0;
	float			t1;
	t_bool			diying;
	t_bool			died;
	int				animation_step;
	int				diying_step;
	t_bool			focus;
	t_collision		hit_data;
	t_bool			shooting;
}					t_entity;

void		entity_update(struct s_doom *doom, t_entity *entity, double dt);
t_bool		create_enemy(struct s_doom *doom, t_renderable *r);
void		compute_enemy_hitbox(t_renderable *r);

#endif
