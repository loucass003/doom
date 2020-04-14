/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_editor_button.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 19:14:40 by Lisa              #+#    #+#             */
/*   Updated: 2020/04/14 19:15:40 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
