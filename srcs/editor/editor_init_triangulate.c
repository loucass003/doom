/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_init_triangulate.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/19 18:08:22 by Lisa              #+#    #+#             */
/*   Updated: 2020/04/19 18:12:22 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "editor.h"

t_triangulate	init_triangulate_floor(int *filter, int i, t_room *room)
{
	t_triangulate	f_c;

	f_c = (t_triangulate) {
		.n = { 0, -1, 0 },
		.filter = filter,
		.filter_len = room->walls->len,
		.normal_type = room->floor_normal,
		.mtl = (i * 2),
		.room_index = i,
		.offset = room->floor_uv_offset,
		.repeat = room->floor_uv_repeat
	};
	return (f_c);
}

t_triangulate	init_triangulate_ceil(int *filter, int i, t_room *room)
{
	t_triangulate	f_c;

	f_c = (t_triangulate) {
		.n = { 0, 1, 0 },
		.filter = filter,
		.filter_len = room->walls->len,
		.normal_type = room->ceil_normal,
		.mtl = (i * 2) + 1,
		.room_index = i,
		.offset = room->ceil_uv_offset,
		.repeat = room->ceil_uv_repeat
	};
	return (f_c);
}
