/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_obj_player.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 22:55:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/19 00:41:42 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor *editor = &doom->editor;

	return (TRUE);
}

void			g_es_obj_player_enter(t_gui *self, t_doom *doom)
{
	int			x = S_WIDTH - 335;
	int			y = 75;

}

void			g_es_obj_player_render(t_gui *self, t_doom *doom)
{
	draw_line(&doom->screen, (t_pixel){ S_WIDTH - 335 + 160, 235, 0xFFFFFF00 }, (t_pixel){ S_WIDTH - 335 + 160, 335, 0 });
}
