/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_player.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 14:07:05 by llelievr          #+#    #+#             */
/*   Updated: 2019/12/03 14:15:14 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "editor.h"



t_bool		editor_render_player(t_doom *doom, t_editor *editor)
{
    if (editor->player_set)
        draw_circle(doom->main_context.image, (t_pixel){editor->player_pos.x, editor->player_pos.z, 0xFFFFFF00}, 7);
}