/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 00:01:14 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/21 03:17:49 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENTITY_H
# define ENTITY_H

# include "libft.h"
# include "collision.h"
# include <al.h>

typedef enum		e_entity_type
{
	ENTITY_PLAYER,
	ENTITY_ENEMY,
	ENTITY_GRENADA
}					t_entity_type;

typedef struct		s_entity_grenada
{
	float			damage;
	float			range;
	long			delay;
}					t_entity_grenada;

typedef union		u_entity_of
{
	t_entity_grenada	grenada;
	// t_entity_enemy		enemy;
}					t_entity_of;


typedef struct		s_entity
{
	t_entity_type	type;
	t_entity_of		of;
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
	ALuint			source[3];

}					t_entity;

void		entity_update(struct s_doom *doom, t_entity *entity, double dt);
t_entity	*create_enemy_entity(struct s_doom *doom);
t_bool		create_enemy(struct s_doom *doom, t_renderable *r);
t_bool		create_grenada(t_renderable *r, struct s_doom *doom);
void		compute_enemy_hitbox(t_renderable *r);

#endif
