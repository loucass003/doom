/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 15:26:38 by lloncham          #+#    #+#             */
/*   Updated: 2020/04/16 21:09:06 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_H
# define PLAYER_H

# include <libft.h>
# include <SDL.h>
# include "entity.h"
# include "camera.h"
# include "item.h"

# define PLAYER_INV_SIZE 8

typedef enum			e_player_state
{
	PS_CROUCH = 0,
	PS_NORMAL = 1,
	PS_SPRINT = 2
}						t_player_state;

typedef struct			s_player_spawn_data
{
	t_vec3				position;
	t_vec3				rotation;
}						t_player_spawn_data;

typedef struct			s_player
{
	t_camera			camera;
	t_entity			entity;
	t_itemstack			item[PLAYER_INV_SIZE];
	int					selected_slot;
	t_player_spawn_data	spawn_data;
	t_player_state		player_state;
	t_player_state		desired_state;
}						t_player;

t_ray					create_shoot_ray(t_player player, t_vec3 direction);
void					update_player_camera(t_player *player);
void					init_player(struct s_doom *doom);
void					spawn_player(struct s_doom *doom);
void					update_controls(struct s_doom *doom);
void					player_inventory_event(struct s_doom *doom,
							SDL_Event *event);
t_bool					draw_player_inventory(struct s_doom *doom, t_gui *self);
int						get_slot_of(t_player *player, t_item_type type);
void					set_player_state(struct s_doom *doom, t_player *player,
							t_player_state state);

#endif
