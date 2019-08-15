/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_ingame.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 11:22:28 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/15 17:43:38 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void static	action_performed(t_component *cmp, t_doom *doom)
{
	if (cmp == doom->guis[doom->current_gui].components[0])
	{
		set_gui(doom, GUI_EDITOR);
	}
}

void	g_ingame_on_enter(t_gui *self, t_doom *doom)
{
}

void	g_ingame_render(t_gui *self, t_doom *doom)
{
	for (int i = 0; i < doom->screen.width * doom->screen.height; i++)
		doom->rendered_area[i] = 0;
	for (int i = 0; i < doom->polygons->len; i++)
		render_polygon(doom, doom->polygons->polygons + i);
	doom->obj_test->position = (t_vec3){2, 0, 2};
	doom->obj_test->rotation.y += 3 * doom->stats.delta;
	doom->obj_test->scale = (t_vec3){0.002, 0.002, 0.002};
	//doom->obj_test->dirty = TRUE;
	//render_obj(doom, doom->obj_test);

}
