/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_wall_sections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 01:20:40 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/20 17:52:52 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "editor.h"
#include "doom.h"

t_wall_section	create_simple_wall_section(t_editor *editor, t_room *room,
	int wall)
{
	t_wall_section	ws;
	int				next;

	next = (wall + 1) % room->walls->len;
	ws = init_wall_section(editor);
	ws.vertices_index[0] = room->room_vertices_start + wall * 2;
	ws.vertices_index[1] = room->room_vertices_start + next * 2;
	ws.vertices_index[2] = room->room_vertices_start + next * 2 + 1;
	ws.vertices_index[3] = room->room_vertices_start + (wall * 2 + 1);
	return (ws);
}

t_wall_section	create_last_wall_section(t_editor *editor, t_room *room,
	int wall, t_gap_filler_packet p)
{
	t_wall_section	ws;
	int				next;

	next = (wall + 1) % room->walls->len;
	ws = init_wall_section(editor);
	ws.vertices_index[0] = editor->rooms->values[(int)p.range_a.z]
		.room_vertices_start + ((int)p.range_a.w * 2) + 1;
	ws.vertices_index[1] = editor->rooms->values[(int)p.range_b.z]
		.room_vertices_start + ((int)p.range_b.w * 2) + 1;
	ws.vertices_index[2] = room->room_vertices_start + next * 2 + 1;
	ws.vertices_index[3] = room->room_vertices_start + (wall * 2 + 1);
	return (ws);
}

void			vertices_index(t_wall_section *ws, t_room *room, int wall,
	int next)
{
	ws->vertices_index[0] = room->room_vertices_start + wall * 2;
	ws->vertices_index[1] = room->room_vertices_start + next * 2;
	ws->vertices_index[2] = room->room_vertices_start + next * 2 + 1;
	ws->vertices_index[3] = room->room_vertices_start + (wall * 2 + 1);
}

t_wall_section	create_between_gaps_wall_section(t_editor *editor, t_room *room,
	int wall, t_gap_filler_packet p)
{
	t_wall_section	ws;
	static t_wall	w0;
	t_wall			w1;
	int				next;

	next = (wall + 1) % room->walls->len;
	w0 = room->walls->values[wall];
	w1 = room->walls->values[next];
	ws = init_wall_section(editor);
	vertices_index(&ws, room, wall, next);
	if (p.range_a.y < w0.ceiling_height)
		ws.vertices_index[3] = editor->rooms->values[(int)p.range_a.z]
			.room_vertices_start + ((int)p.range_a.w * 2) + 1;
	if (p.range_b.y < w1.ceiling_height)
		ws.vertices_index[2] = editor->rooms->values[(int)p.range_b.z]
			.room_vertices_start + ((int)p.range_b.w * 2) + 1;
	if (p.range_a.x > w0.floor_height)
		ws.vertices_index[0] = editor->rooms->values[(int)p.range_a.z]
			.room_vertices_start + (int)p.range_a.w * 2;
	if (p.range_b.x > w1.floor_height)
		ws.vertices_index[1] = editor->rooms->values[(int)p.range_b.z]
			.room_vertices_start + (int)p.range_b.w * 2;
	return (ws);
}

t_wall_section	create_gap_wall_section(t_editor *editor, t_room *room,
	int wall, t_gap_filler_packet p)
{
	t_wall_section	ws;
	t_wall			w0;
	t_wall			w1;
	int				next;

	next = (wall + 1) % room->walls->len;
	w0 = room->walls->values[wall];
	w1 = room->walls->values[next];
	ws = init_wall_section(editor);
	ws.vertices_index[0] = room->room_vertices_start + wall * 2;
	ws.vertices_index[1] = room->room_vertices_start + next * 2;
	ws.vertices_index[3] = editor->rooms->values[(int)p.range_a.z]
		.room_vertices_start + ((int)p.range_a.w * 2);
	ws.vertices_index[2] = editor->rooms->values[(int)p.range_b.z]
		.room_vertices_start + ((int)p.range_b.w * 2);
	if (p.start_a > w0.floor_height)
		ws.vertices_index[0] = editor->rooms->values[(int)p.last_range_a.z]
			.room_vertices_start + (int)p.last_range_a.w * 2 + 1;
	if (p.start_b > w1.floor_height)
		ws.vertices_index[1] = editor->rooms->values[(int)p.last_range_b.z]
			.room_vertices_start + (int)p.last_range_b.w * 2 + 1;
	return (ws);
}
