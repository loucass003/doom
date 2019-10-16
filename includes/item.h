/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   item.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/11 20:30:43 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/15 19:48:25 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITEM_H
# define ITEM_H

# include <SDL.h>
# include <libft.h>

typedef enum		e_weapon_type
{
	WEAPON_GUN,
	WEAPON_AR
}					t_weapon_type;

typedef struct		s_weapon
{
	t_weapon_type	type;
	float			damage;
	float			fire_cooldown;
	SDL_Surface		*animation;
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
	ITEM_UNKNOWN,
	ITEM_WEAPON,
	ITEM_AMMO,
	ITEM_HEAL,
	ITEM_KEY,
	ITEM_JETPACK
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
	SDL_Surface		*image;
	void			(*on_use)(struct s_doom *doom, struct s_itemstack *is);
	int				max_stack_size;
}					t_item;

typedef struct		s_itemstack
{
	t_item			*of;
	int				amount;
}					t_itemstack;

t_bool				create_itemstack_renderable(t_renderable *r, t_item *item, int amount);
t_item				*create_item(t_item_type type, SDL_Surface *image, SDL_Rect bounds, int max_stack_size);
t_item				*create_item_weapon(SDL_Surface *image, SDL_Rect bounds, t_weapon_type type);
t_item				*create_item_weapon_gun(SDL_Surface *image, SDL_Surface *animation);
t_item				*create_item_ammo(SDL_Surface *image);
t_item				*create_item_heal(SDL_Surface *image);
t_bool				entity_hit_itemstack(struct s_entity *entity, t_itemstack *is);

#endif