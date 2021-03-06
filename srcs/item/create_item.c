/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_item.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 14:43:20 by lloncham          #+#    #+#             */
/*   Updated: 2020/02/19 18:07:52 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <limits.h>
#include "sprite.h"
#include "doom.h"
#include "item.h"

t_item	*create_item(t_item_type type, t_ressource *image, SDL_Rect bounds,
	int max_stack_size)
{
	t_item	*item;

	if (!(item = ft_memalloc(sizeof(t_item))))
		return (NULL);
	item->type = type;
	item->image = image;
	item->bounds = bounds;
	item->max_stack_size = max_stack_size;
	return (item);
}

t_item	*create_item_weapon(t_ressource *image, SDL_Rect bounds,
	t_weapon_type type, int max_stack_size)
{
	t_item	*weapon;

	if (!(weapon = create_item(ITEM_WEAPON, image, bounds, max_stack_size)))
		return (NULL);
	weapon->data.weapon.type = type;
	weapon->data.weapon.damage = 0.6;
	return (weapon);
}

t_item	*create_item_jetpack(t_ressource *image)
{
	t_item	*jetpack;

	if (!(jetpack = create_item(ITEM_JETPACK, image,
		(SDL_Rect){ 257, 257, 146, 103 }, INT_MAX)))
		return (NULL);
	return (jetpack);
}

t_item	*create_item_ammo(t_ressource *image)
{
	t_item	*ammo;

	if (!(ammo = create_item(ITEM_AMMO, image,
		(SDL_Rect){ 1, 1, 256, 256 }, 75)))
		return (NULL);
	return (ammo);
}

t_item	*create_item_heal(t_ressource *image)
{
	t_item	*heal;

	if (!(heal = create_item(ITEM_HEAL, image,
		(SDL_Rect){ 126, 259, 100, 100 }, 1)))
		return (NULL);
	return (heal);
}
