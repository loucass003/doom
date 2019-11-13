/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 01:20:07 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/13 17:01:03 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "editor.h"
#include "write_structs.h"

t_bool		write_wall(t_ressource_manager *r, t_wall *wall)
{
	const t_wr_wall	wr_wall = (t_wr_wall) {
		.indice = wall->indice,
		.resource_index = ressources_indexof(r->ressources, wall->texture),
		.normal_type = wall->normal_type,
		.invisible = wall->invisible,
		.collisions = wall->collisions
	};

	dp_write(r, &wr_wall, sizeof(t_wr_wall));
	return (TRUE);
}

t_bool		write_room(t_ressource_manager *r, t_room *room)
{
	int				i;
	const t_wr_room	wr_room = (t_wr_room) {
		.closed = room->closed,
		.walls_count = room->walls->len
	};

	dp_write(r, &wr_room, sizeof(t_wr_room));
	i = -1;
	while (++i < room->walls->len)
		if (!write_wall(r, &room->walls->values[i]))
			return (FALSE);
	return (TRUE);
}

t_bool		write_rooms(t_ressource_manager *r)
{
	int		i;

	dp_write(r, &r->doom->editor.rooms->len, sizeof(int));
	i = -1;
	while (++i < r->doom->editor.rooms->len)
	{
		if (!write_room(r, &r->doom->editor.rooms->values[i]))
			return (FALSE);
	}
	return (TRUE);
}

t_bool		write_points(t_ressource_manager *r)
{
	int		i;

	dp_write(r, &r->doom->editor.points->len, sizeof(int));
	dp_write(r, &r->doom->editor.points->vertices,
		sizeof(t_vec2) * r->doom->editor.points->len);
}

t_bool		write_map(t_ressource_manager *r)
{
	write_points(r);
	write_rooms(r);
}
