/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 00:01:14 by llelievr          #+#    #+#             */
<<<<<<< Updated upstream
/*   Updated: 2020/01/20 14:43:19 by louali           ###   ########.fr       */
=======
/*   Updated: 2020/01/21 15:05:31 by lloncham         ###   ########.fr       */
>>>>>>> Stashed changes
/*                                                                            */
/* ************************************************************************** */

#ifndef ENTITY_H
# define ENTITY_H

# include <libft.h>
# include "collision.h"
# include "render.h"
# include <al.h>

typedef enum		e_entity_type
{
	ENTITY_PLAYER,
	ENTITY_ENEMY,
	ENTITY_GRENADA,
	ENTITY_BOSS,
	ENTITY_ROCKET
}					t_entity_type;

typedef struct		s_entity_grenada
{
	float			damage;
	float			range;
	long			delay;
}					t_entity_grenada;

typedef struct		s_entity_rocket
{
	float			damage;
	float			range;
	ALuint			sources[1];
}					t_entity_rocket;

typedef struct		s_entity_enemy
{
	float			t0;
	float			t1;
	int				animation_step;
	int				diying_step;
	t_bool			focus;
	t_collision		hit_data;
	t_bool			shooting;
	ALuint			sources[3];
}					t_entity_enemy;

typedef struct		s_entity_boss
{
	float			t0;
	float			t1;
	float			t2;
	t_bool			shooting;
	t_collision		hit_data;
	t_bool			focus;
	int				animation_step;
	int				phase;
	ALuint			sources[4];
	int				shoot;
}					t_entity_boss;

typedef union		u_entity_of
{
	t_entity_grenada	grenada;
	t_entity_enemy		enemy;
	t_entity_boss		boss;
	t_entity_rocket		rocket;
}					t_entity_of;

typedef struct		s_entity
{
	t_entity_type		type;
	t_entity_of			of;
	t_vec3				position;
	t_vec3				velocity;
	t_vec3				rotation;
	t_vec3				scale;
	t_vec3				radius;
	t_bool				grounded;
	t_bool				jump;
	t_bool				dead;
	t_bool				diying;
	t_physics_data		packet;
	float				life;
	float				max_life;
	ALuint				*sources;
	t_bool				jetpack;
	struct s_renderable	*r;
}					t_entity;

t_bool		entity_update(struct s_doom *doom, t_entity *entity, double dt);
t_bool		create_enemy_renderable(struct s_doom *doom, t_renderable *r);
t_bool		create_boss_renderable(struct s_doom *doom, t_renderable *r);
t_bool		create_grenada(t_renderable *r, struct s_doom *doom);
void		compute_entity_hitbox(t_renderable *r);
void		entity_update_boss(struct s_doom *doom, t_entity *entity, double dt);
void		entity_update_enemy(struct s_doom *doom, t_entity *entity, double dt);

t_bool	renderable_rocket(struct s_doom *doom, t_vec3 from, t_vec3 to);
t_bool	create_rocket(t_renderable *r, struct s_doom *doom);


#endif
