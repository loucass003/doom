/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   item.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/11 20:30:43 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/10 14:39:05 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITEM_H
# define ITEM_H

# include <SDL.h>
# include <libft.h>
# include "ressource.h"

typedef enum		e_weapon_type
{
	WEAPON_GUN = 0,
	WEAPON_AXE = 1,
	WEAPON_GRENADA = 2,
	WEAPON_AR = 3
}					t_weapon_type;

typedef struct		s_weapon
{
	t_weapon_type	type;
	float			damage;
	float			fire_cooldown;
	t_ressource		*animation;
	SDL_Rect		bounds;
	t_vec2			cells;
	uint8_t			*animation_seq;
	int				current_step;
	t_bool			fireing;
	int				steps_count;
	SDL_Rect		curr_image;
	int				idle_step;
}					t_weapon;

typedef enum		e_item_type
{
	ITEM_UNKNOWN = -1,
	ITEM_AMMO = 0,
	ITEM_WEAPON = 1,
	ITEM_HEAL = 2,
	ITEM_KEY = 3,
	ITEM_JETPACK = 4
}					t_item_type;

typedef union		u_item_data
{
	t_weapon		weapon;
}					t_item_data;

typedef struct		s_item
{
	t_item_type		type;
	t_item_data		data;
	SDL_Rect		bounds;
	t_ressource		*image;
	void			(*on_use)(struct s_doom *doom, struct s_itemstack *is);
	int				max_stack_size;
}					t_item;

typedef struct		s_itemstack
{
	t_item			*of;
	int				amount;
}					t_itemstack;


t_bool				create_itemstack_renderable(t_renderable *r, t_item *item, int amount);
t_item				*create_item(t_item_type type, t_ressource *image, SDL_Rect bounds, int max_stack_size);
t_item				*create_item_weapon(t_ressource *image, SDL_Rect bounds, t_weapon_type type, int max_stack_size);
t_item				*create_item_weapon_gun(t_ressource *image, t_ressource *animation);
t_item				*create_item_weapon_grenada(t_ressource *image);
t_item				*create_item_ammo(t_ressource *image);
t_item				*create_item_heal(t_ressource *image);
t_item				*create_item_jetpack(t_ressource *image);
t_bool				entity_hit_itemstack(struct s_entity *entity, t_itemstack *is);
t_item				*create_item_weapon_axe(t_ressource *image, t_ressource *animation);
t_itemstack			*create_itemstack(t_item *item, int amount);
t_itemstack			*create_itemstack_from_type(t_doom *doom, t_item_type type, t_weapon_type weapon_type);
t_item				*create_item_weapon_from_type(t_doom *doom, t_weapon_type type);
t_item				*create_item_from_type(t_doom *doom, t_item_type type, t_weapon_type weapon_type);
void				set_current_animation_step(t_weapon *weapon, int i);


#endif
