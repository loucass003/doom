/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_player.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 14:07:05 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/13 15:18:14 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "editor.h"

t_bool		editor_render_player(t_doom *doom, t_editor *editor)
{
	if (editor->player_set)
	{
		draw_circle(doom->main_context.image,
			(t_pixel){
				doom->player.spawn_data.position.x,
				doom->player.spawn_data.position.z,
				0xFFFFFF00
			},
		7);
	}
	return (TRUE);
}
