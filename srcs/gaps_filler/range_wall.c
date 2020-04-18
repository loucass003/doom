/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   range_wall.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/19 00:49:56 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/19 00:53:16 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_bool		is_start_range(t_wall wall2, t_wall *wall0)
{
	return ((wall2.floor_height > wall0->floor_height
		&& wall2.floor_height < wall0->ceiling_height)
		|| (wall2.ceiling_height > wall0->floor_height
		&& wall2.ceiling_height < wall0->ceiling_height));
}

t_bool		is_end_range(t_wall wall3, t_wall *wall1)
{
	return ((wall3.floor_height > wall1->floor_height
		&& wall3.floor_height < wall1->ceiling_height)
		|| (wall3.ceiling_height > wall1->floor_height
		&& wall3.ceiling_height < wall1->ceiling_height));
}

t_bool		append_in_range(t_wall *wall[4], int j, int index1, int index0)
{
	if (is_start_range(*wall[2], wall[0]))
	{
		if (!wall[0]->start_rooms_range
			&& !(wall[0]->start_rooms_range = create_4dvertices_array(5)))
			return (FALSE);
		append_4dvertices_array(&wall[0]->start_rooms_range, (t_vec4){
				wall[2]->floor_height, wall[2]->ceiling_height, j, index0 });
	}
	if (is_end_range(*wall[3], wall[1]))
	{
		if (!wall[1]->end_rooms_range
			&& !(wall[1]->end_rooms_range = create_4dvertices_array(5)))
			return (FALSE);
		append_4dvertices_array(&wall[1]->end_rooms_range, (t_vec4){
			wall[3]->floor_height, wall[3]->ceiling_height, j, index1 });
	}
	return (TRUE);
}

int			range_wall(t_wall *wall0, t_wall *wall1, t_editor *editor, int j)
{
	t_room	*curr_room;
	int		index0;
	int		index1;
	t_wall	wall2;
	t_wall	wall3;

	curr_room = &editor->rooms->values[j];
	index0 = wall_indexof_by_indice(curr_room->walls, wall0->indice);
	index1 = wall_indexof_by_indice(curr_room->walls, wall1->indice);
	if (index0 == -1 || index1 == -1)
		return (0);
	wall2 = editor->rooms->values[j].walls->values[index0];
	wall3 = editor->rooms->values[j].walls->values[index1];
	if (!append_in_range((t_wall*[4]){ wall0, wall1, &wall2, &wall3 }, j,
		index1, index0))
		return (-1);
	return (1);
}
