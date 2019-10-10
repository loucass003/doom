#ifndef PLAYER_H
# define PLAYER_H

#include "libft.h"
#include "maths/vec4.h"
#include "collision.h"
#include "polygon.h"
#include "entity.h"
#include "camera.h"

typedef struct					s_player
{
	t_camera		camera;
	t_entity		entity;
}								t_player;

t_ray				create_shoot_ray(t_player player, t_vec3 direction);

void				update_player_camera(t_player *player);
void				init_player(struct s_doom *doom);

#endif
