#ifndef PLAYER_H
# define PLAYER_H

#include "libft.h"
#include "maths/vec4.h"
#include "collision.h"
#include "polygon.h"
#include "camera.h"

typedef struct					s_player
{
	t_vec3			pos;
	t_vec3			rotation;
	t_camera		camera;
	t_ray			ray;
	t_bool			fixed_ray;
	t_collide_aabb	aabb;
	t_bool			coliding;
}								t_player;

t_ray				create_shoot_ray(t_player player, t_vec3 direction);

void				update_player_camera(t_player *player);

#endif
