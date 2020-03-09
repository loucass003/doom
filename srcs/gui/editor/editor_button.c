/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_button.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 15:17:54 by lloncham          #+#    #+#             */
/*   Updated: 2020/03/09 02:43:14 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "gui.h"
#include "doom.h"

t_bool					selected_tool(t_doom *doom, int last_select)
{
	if (doom->editor.selected_tool == TOOL_PLAYER
		|| (last_select == TOOL_PLAYER
			&& last_select != doom->editor.selected_tool))
		editor_settings_update(&doom->editor);
	if (doom->editor.selected_tool == 0)
	{
		if (!doom->editor.player_set)
			return (TRUE);
		doom->editor.selected_tool = -1;
		doom->main_context.type = CTX_EDITOR;
		init_player(doom);
		spawn_player(doom);
		unselect_all(doom);
		free_renderables(&doom->renderables, FALSE);
		editor_setmap(&doom->editor);
		set_gui(doom, GUI_INGAME);
		return (FALSE);
	}
	return (TRUE);
}

void					tool_current(t_doom *doom)
{
	t_room	*curr_room;

	if (doom->editor.current_room != -1)
	{
		curr_room = &doom->editor.rooms->values[doom->editor.current_room];
		curr_room->closed = TRUE;
		select_room(&doom->editor, -1);
		doom->editor.line_start_cell = (t_vec2){ -1, -1 };
	}
	if (doom->editor.current_object != -1)
	{
		doom->editor.current_object = -1;
		editor_settings_update(&doom->editor);
	}
}

static t_bool			tools_action_performed(t_component *cmp, t_doom *doom)
{
	int			i;
	t_button	*btn;
	int			last_select;

	btn = (t_button*)cmp;
	i = -1;
	last_select = doom->editor.selected_tool;
	while (++i < 6)
	{
		btn = (t_button*)doom->guis[doom->current_gui].components->values[i];
		if ((t_button*)cmp == btn)
		{
			doom->editor.selected_tool = i;
			btn->selected = TRUE;
		}
		else
			btn->selected = FALSE;
	}
	tool_current(doom);
	if (selected_tool(doom, last_select) != TRUE)
		return (FALSE);
	return (TRUE);
}

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	if (cmp == doom->guis[doom->current_gui].components->values[6])
	{
		doom->editor.settings.open = !doom->editor.settings.open;
		((t_button *)doom->guis[doom->current_gui].components->values[6])
			->selected = doom->editor.settings.open;
	}
	return (TRUE);
}

void					g_editor_button(t_gui *self, t_doom *doom)
{
	int		i;
	t_img	*img;

	img = doom->res_manager.ressources->values[25]->data.texture;
	append_components_array(&self->components, create_button((SDL_Rect)
		{8, 9, 50, 50}, img, NULL));
	append_components_array(&self->components, create_button((SDL_Rect)
		{60, 9, 50, 50}, img, NULL));
	append_components_array(&self->components, create_button((SDL_Rect)
		{112, 9, 50, 50}, img, NULL));
	append_components_array(&self->components, create_button((SDL_Rect)
		{164, 9, 50, 50}, img, NULL));
	append_components_array(&self->components, create_button((SDL_Rect)
		{216, 9, 50, 50}, img, NULL));
	append_components_array(&self->components, create_button((SDL_Rect)
		{268, 9, 50, 50}, img, NULL));
	append_components_array(&self->components, create_button((SDL_Rect)
		{S_WIDTH - 57, 9, 50, 50}, img, NULL));
	((t_button *)self->components->values[6])->selected = doom->editor
		.settings.open;
	i = -1;
	while (++i < 7)
	{
		((t_button *)self->components->values[i])->cells_count = (t_vec2){7, 1};
		set_button_cell(((t_button *)self->components->values[i]), i, 0);
		self->components->values[i]->perform_action = tools_action_performed;
	}
	self->components->values[6]->perform_action = action_performed;
}
