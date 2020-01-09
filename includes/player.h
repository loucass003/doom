#ifndef PLAYER_H
# define PLAYER_H

#include "libft.h"
#include <SDL.h>
#include "entity.h"
#include "camera.h"
#include "item.h"

#define PLAYER_INV_SIZE (8)

typedef struct					s_player_spawn_data
{
	t_vec3		position;
	t_vec3		rotation;
}								t_player_spawn_data;

typedef struct					s_player
{
	t_camera			camera;
	t_entity			entity;
	t_itemstack			item[PLAYER_INV_SIZE];
	int					selected_slot;
	t_player_spawn_data	spawn_data;
}								t_player;

t_ray				create_shoot_ray(t_player player, t_vec3 direction);
void				update_player_camera(t_player *player);
void				init_player(struct s_doom *doom);
void				spawn_player(struct s_doom *doom);
void				update_controls(struct s_doom *doom);
void				player_inventory_event(struct s_doom *doom, SDL_Event *event);
t_bool				draw_player_inventory(struct s_doom *doom, t_gui *self);

#endif
