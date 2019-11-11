/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_settings.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/11 18:01:41 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/11 18:02:54 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	editor_settings_update(t_editor *editor)
{
	if (editor->current_room != -1 && editor->rooms->values[editor->current_room].closed && editor->grid_cell_grab == GG_LINE)
		editor->settings_visible = TRUE;
	else
	{
		editor->settings_visible = FALSE;
		editor->current_seg = (t_vec2){ -1, -1 };
	}
}
