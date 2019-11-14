/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_settings.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/11 18:01:41 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/14 05:22:26 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

void	editor_settings_update(t_editor *editor)
{
	if (editor->current_room != -1 && editor->rooms->values[editor->current_room].closed )
	{
		editor->settings_visible = TRUE;
		t_gui	*gui = &editor->doom->guis[GUI_EDITOR_SETTINGS];
		t_wall	*wall = get_current_wall(editor);
		int		i;

		i = -1;
		while (++i < gui->components->len)
		{
			if (editor->current_seg.x != -1 && i < ES_WALL_COMPONENTS)
			{
				gui->components->values[i]->visible = TRUE;
				gui->components->values[i]->enabled = TRUE;
				if (i == 2)
					((t_checkbox *)gui->components->values[i])->value = wall->collisions;
				else if (i == 3)
					((t_checkbox *)gui->components->values[i])->value = wall->invisible;
			}
			else if (editor->current_seg.x == -1 && i >= ES_WALL_COMPONENTS && i < ES_ROOM_COMPONENTS)
			{
				gui->components->values[i]->visible = TRUE;
				gui->components->values[i]->enabled = TRUE;
			}
			else
			{
				gui->components->values[i]->visible = FALSE;
				gui->components->values[i]->enabled = FALSE;
			}
		}
	}
	else
	{
		editor->settings_visible = FALSE;
		editor->current_seg = (t_vec2){ -1, -1 };
	}
}
