/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_editor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 15:50:09 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/15 14:42:06 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "gui.h"
#include "doom.h"

static t_bool		tools_action_performed(t_component *cmp, t_doom *doom)
{
	int			i;
	t_button	*btn;

	btn = cmp;
	i = -1;
	while (++i < 4)
	{
		btn = doom->guis[doom->current_gui].components->values[i];
		if (cmp == btn)
		{
			doom->editor.selected_tool = i;
			btn->selected = TRUE;
		}
		else
			btn->selected = FALSE;
	}
	if (doom->editor.current_room != -1)
	{
		t_room	*curr_room = &doom->editor.rooms->values[doom->editor.current_room];
		curr_room->closed = TRUE;
		select_room(&doom->editor, -1);
		doom->editor.line_start_cell = (t_vec2){ -1, -1 };
	}
	if (doom->editor.selected_tool == 0)
	{
		doom->editor.selected_tool = -1;
		set_gui(doom, GUI_INGAME);
		return (FALSE);
	}
	return (TRUE);
}

t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	if (cmp == doom->guis[doom->current_gui].components->values[4])
	{
		doom->editor.settings_open = !doom->editor.settings_open;
		((t_button *)doom->guis[doom->current_gui].components->values[4])->selected = doom->editor.settings_open;
	}
	return (TRUE);
} 

void			g_editor_button(t_gui *self, t_doom *doom)
{
	append_components_array(&self->components, create_button((SDL_Rect){8, 9, 50, 50}, "icons/icons8-jouer-80.png", NULL));
	append_components_array(&self->components, create_button((SDL_Rect){60, 9, 50, 50}, "icons/poly.png", NULL));
	append_components_array(&self->components, create_button((SDL_Rect){112, 9, 50, 50}, "icons/point.png", NULL));
	append_components_array(&self->components, create_button((SDL_Rect){164, 9, 50, 50}, "icons/curseur3.png", NULL));
	int i = 0;
	while (i < 4)
		self->components->values[i++]->perform_action = tools_action_performed;
	append_components_array(&self->components, create_button((SDL_Rect){S_WIDTH - 57, 9, 50, 50}, "icons/settings.png", NULL));
	self->components->values[self->components->len - 1]->perform_action = action_performed;
	((t_button *)self->components->values[self->components->len - 1])->selected = TRUE;;
}

void			g_editor_on_enter(t_gui *self, t_doom *doom)
{
	
	doom->editor.line_start_cell = (t_vec2){ -1, -1 };
	doom->editor.current_point = -1;
	select_room(&doom->editor, -1);
	doom->editor.settings_open = TRUE;

	g_editor_button(self, doom);
}

void			g_editor_on_leave(t_gui *self, t_doom *doom)
{

}

void			g_editor_on_event(t_gui *self, SDL_Event *event, t_doom *doom)
{
	if (event->type == SDL_MOUSEMOTION)
	{
		t_vec2 pos = (t_vec2){ event->motion.x, event->motion.y };
		if (in_bounds((SDL_Rect){ 10, 70, doom->screen.width - 20, doom->screen.height - 80 }, pos))
			doom->editor.grid_cell = get_close_point(&doom->editor, pos);
		else
		{
			doom->editor.grid_cell_grab = GG_OUTSIDE;
			doom->editor.grid_cell = (t_vec2){ -1, -1 };
		}
		
		if ((doom->editor.selected_tool == TOOL_POINT || doom->editor.selected_tool == TOOL_SELECT) && doom->editor.grid_cell.x != -1 && doom->editor.current_point != -1)
			editor_tool_point_move(&doom->editor);
	}
	else if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		if (doom->editor.selected_tool == TOOL_ROOM && doom->editor.grid_cell.x != -1)
			editor_tool_room(&doom->editor, event);
		else if (doom->editor.selected_tool == TOOL_POINT)
			editor_tool_point(&doom->editor);
		else if (doom->editor.selected_tool == TOOL_SELECT)
			editor_tool_select(&doom->editor, event);
	}
	else if (event->type == SDL_MOUSEBUTTONUP)
	{
		if (doom->editor.selected_tool == TOOL_POINT || doom->editor.selected_tool == TOOL_SELECT)
			editor_tool_point_release(&doom->editor);
	}
	else if (event->type == SDL_KEYDOWN)
	{
		if (event->key.keysym.scancode == SDL_SCANCODE_DELETE)
			editor_delete_action(&doom->editor);
	}
}

void			g_editor_render(t_gui *self, t_doom *doom)
{
	int				x;
	int				y;

	//printf("POINTS %d\n", doom->editor.points->len);

	editor_grid_render(self, doom, &doom->editor);
	draw_rect(&doom->screen, (SDL_Rect){ 5, 5, S_WIDTH - 10, 57 }, 0xFFFFFFFF);
	render_components(doom, self);
	doom->guis[GUI_EDITOR_SETTINGS].render(&doom->guis[GUI_EDITOR_SETTINGS], doom);
	// show_new_components(doom);
}
