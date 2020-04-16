/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_event.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 14:53:54 by lloncham          #+#    #+#             */
/*   Updated: 2020/04/16 04:49:57 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gui.h"
#include "doom.h"

void					editor_mousemotion(SDL_Event *event, t_doom *doom)
{
	t_vec2 pos;

	pos = (t_vec2){ event->motion.x, event->motion.y };
	if (in_bounds((SDL_Rect){ 10, 70, doom->screen.width - 20,
		doom->screen.height - 80 }, pos))
		doom->editor.grid_cell = get_close_point(&doom->editor, pos);
	else
	{
		doom->editor.grid_cell_grab = GG_OUTSIDE;
		doom->editor.grid_cell = (t_vec2){ -1, -1 };
	}
	if ((doom->editor.selected_tool == TOOL_POINT
		|| doom->editor.selected_tool == TOOL_SELECT)
		&& doom->editor.grid_cell.x != -1 && doom->editor.current_point != -1)
		editor_tool_point_move(&doom->editor);
	if (doom->editor.current_object != -1 && doom->editor.object_grab)
		editor_tool_objects_move(&doom->editor);
	if (doom->editor.player_grab)
		editor_tool_player_move(&doom->editor);
}

void					editor_mousebuttonup(t_doom *doom)
{
	if (doom->editor.selected_tool == TOOL_POINT
		|| doom->editor.selected_tool == TOOL_SELECT)
		editor_tool_point_release(&doom->editor);
	if (doom->editor.selected_tool == TOOL_SELECT
		|| doom->editor.selected_tool == TOOL_OBJECTS)
		editor_tool_objects_release(&doom->editor);
	if (doom->editor.selected_tool == TOOL_SELECT
		|| doom->editor.selected_tool == TOOL_PLAYER)
		editor_tool_player_release(&doom->editor);
}

void					editor_mousebuttondown(SDL_Event *event, t_doom *doom)
{
	if (doom->editor.selected_tool == TOOL_ROOM
		&& doom->editor.grid_cell.x != -1)
		editor_tool_room(&doom->editor, event);
	else if (doom->editor.selected_tool == TOOL_POINT)
		editor_tool_point(&doom->editor);
	else if (doom->editor.selected_tool == TOOL_SELECT)
		editor_tool_select(&doom->editor);
	else if (doom->editor.selected_tool == TOOL_OBJECTS)
		editor_tool_objects(&doom->editor);
	else if (doom->editor.selected_tool == TOOL_PLAYER)
		editor_tool_player(&doom->editor);
}

void					editor_keydown(SDL_Event *event, t_doom *doom)
{
	if (event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
		editor_delete_action(&doom->editor);
	if (event->key.keysym.scancode == SDL_SCANCODE_EQUALS)
		remove_point(&doom->editor, 0);
}
