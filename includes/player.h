#ifndef PLAYER_H
# define PLAYER_H

#include "libft.h"
#include "maths/vec4.h"
#include "collision.h"
#include "polygon.h"

typedef struct					s_player
{
	t_vec3			pos;
	t_ray			ray;
	t_bool			fixed_ray;
	t_mat4			matrix;
	t_mat4			projection;
	t_vec2			rotation;
	t_collide_aabb	aabb;
	t_bool			coliding;
	t_polygon		*pointed_poly;
	int				pointed_triangle;
}								t_player;

t_ray				create_shoot_ray(t_player player, t_vec3 direction);

#endif