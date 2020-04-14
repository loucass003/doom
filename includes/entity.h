/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 00:01:14 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/13 17:35:11 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENTITY_H
# define ENTITY_H

# include <libft.h>
# include "collision.h"
# include "render.h"
# include <al.h>

typedef enum			e_entity_type
{
	ENTITY_PLAYER,
	ENTITY_ENEMY,
	ENTITY_GRENADA,
	ENTITY_BOSS,
	ENTITY_ROCKET
}						t_entity_type;

typedef struct			s_entity_grenada
{
	float				damage;
	float				range;
	long				delay;
	long				start;
}						t_entity_grenada;

typedef struct			s_entity_rocket
{
	float				damage;
	float				range;
	t_mat4				look_at;
	ALuint				sources[1];
}						t_entity_rocket;

typedef struct			s_entity_enemy
{
	float				t0;
	float				t1;
	int					animation_step;
	int					diying_step;
	t_bool				focus;
	t_collision			hit_data;
	t_bool				shooting;
	ALuint				sources[3];
}						t_entity_enemy;

typedef struct			s_entity_boss
{
	float				t0;
	float				t1;
	float				t2;
	t_bool				shooting;
	t_collision			hit_data;
	t_bool				focus;
	int					animation_step;
	int					phase;
	ALuint				sources[4];
	int					shoot;
}						t_entity_boss;

typedef union			u_entity_of
{
	t_entity_grenada	grenada;
	t_entity_enemy		enemy;
	t_entity_boss		boss;
	t_entity_rocket		rocket;
}						t_entity_of;

typedef struct			s_entity
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
	t_bool				killable;
	t_bool				crouch;
	t_bool				run;
	struct s_renderable	*r;
	t_bool				ontp;

}						t_entity;

t_bool					entity_update(struct s_doom *doom, t_entity *entity,
							double dt);
t_bool					create_enemy_renderable(struct s_doom *doom,
							t_renderable *r);

t_bool					create_boss_renderable(struct s_doom *doom,
							t_renderable *r);
void					boss_shoot(struct s_doom *doom, t_entity *entity,
							t_entity_boss *boss);
void					boss_shoot_true(struct s_doom *doom, t_entity *entity,
							t_entity_boss *boss);
void					boss_hit_dist(struct s_doom *doom, t_entity *entity,
							t_entity_boss *boss);
void					set_boss_focus(t_entity *entity, t_entity_boss *boss);
void					boss_rocket(struct s_doom *doom, t_entity *entity,
							t_entity_boss *boss);
void					boss_hit_dist_step(struct s_doom *doom,
							t_entity *entity, t_entity_boss *boss);
void					boss_anim_state(struct s_doom *doom, t_entity *entity,
							t_entity_boss *boss);
void					boss_update_step(struct s_doom *doom, t_entity *entity,
							t_entity_boss *boss, double dt);

t_bool					create_grenada(t_renderable *r, struct s_doom *doom);
void					compute_entity_hitbox(t_renderable *r);
void					entity_update_boss(struct s_doom *doom,
							t_entity *entity, double dt);

void					entity_update_enemy(struct s_doom *doom,
							t_entity *entity, double dt);
void					entity_update_sound(t_entity *entity);
void					enemy_focus_step(struct s_doom *doom, t_entity *entity,
							t_entity_enemy *enemy, t_bool walking);
void					update_entity_state(struct s_doom *doom,
							t_entity *entity, t_entity_enemy *enemy);
void					set_enemy_focus(struct s_doom *doom, t_entity *entity,
							t_entity_enemy *enemy, t_vec3 norm_dir);
void					entity_anim_step(struct s_doom *doom, t_entity *entity,
							t_entity_enemy *enemy);

t_bool					entity_update_grenada(struct s_doom *doom,
							t_entity *entity);
t_vec3					collide_with_world(t_entity *entity, t_vec3 e_position,
							t_vec3 e_velocity, t_bool *stop);

t_bool					renderable_rocket(struct s_doom *doom, t_vec3 from,
							t_vec3 to);
t_bool					create_rocket(t_renderable *r, struct s_doom *doom);

t_bool					create_explosion_renderable(struct s_doom *doom,
							t_renderable *r);
void					entity_update_explosion(struct s_doom *doom,
							t_entity *entity, double dt);

t_bool					update_entity_type(struct s_doom *doom,
							t_entity *entity, double dt);

#endif
