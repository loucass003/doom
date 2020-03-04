/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_map_read_itemstack.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 17:22:20 by lloncham          #+#    #+#             */
/*   Updated: 2020/03/04 17:22:41 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "ressource.h"
#include "write_structs.h"
#include "sprite.h"

t_bool		read_item(t_ressource_manager *r, t_item **item)
{
	t_item_type		item_type;
	t_weapon_type	weapon_type;

	item_type = ITEM_UNKNOWN;
	weapon_type = -1;
	if (!io_memcpy(&r->reader, &item_type, sizeof(t_item_type)))
		return (FALSE);
	if (item_type == ITEM_WEAPON && !io_memcpy(&r->reader, &weapon_type,
		sizeof(t_weapon_type)))
		return (FALSE);
	*item = create_item_from_type(r->doom, item_type, weapon_type);
	if (!*item)
		return (FALSE);
	return (TRUE);
}

t_bool		read_itemstack(t_ressource_manager *r, t_itemstack **is)
{
	int				amount;
	t_item			*item;

	if (!io_memcpy(&r->reader, &amount, sizeof(int)))
		return (FALSE);
	if (!read_item(r, &item))
		return (FALSE);
	if (!(*is = create_itemstack(item, amount)))
		return (FALSE);
	return (TRUE);
}
