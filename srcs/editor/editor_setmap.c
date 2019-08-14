/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_setmap.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 13:54:34 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/14 14:02:08 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void set_map(t_doom *doom)
{
	printf("LOL\n");

	t_polygons *polygons = create_polygons_array(doom->editor.polygon->len + 2);
	
	t_3dvertices *vertices = create_3dvertices_array(doom->editor.polygon->len);
	for (int i = 0; i < doom->editor.polygon->len; i++)
	{
		append_3dvertices_array(&vertices, (t_vec3){
			doom->editor.polygon->values[doom->editor.polygon->len - 1 - i].line.a.x, 0, doom->editor.polygon->values[doom->editor.polygon->len - 1 - i].line.a.y});
	}
	append_polygons_array(&polygons, create_polygon(vertices, P_FLOOR));
	
	t_3dvertices *vertices_ceil = create_3dvertices_array(doom->editor.polygon->len);
	for (int i = 0; i < doom->editor.polygon->len; i++)
	{
		append_3dvertices_array(&vertices_ceil, (t_vec3){
			doom->editor.polygon->values[i].line.a.x, 1.5, doom->editor.polygon->values[i].line.a.y});
	}
	append_polygons_array(&polygons, create_polygon(vertices_ceil, P_CEILING));
	
	for (int i = 0; i < doom->editor.polygon->len; i++)
	{
		append_polygons_array(&polygons, create_wall_polygon((t_line){ (t_vec2){doom->editor.polygon->values[i].line.a.x, (doom->editor.polygon->values[i].line.a.y)}, (t_vec2){doom->editor.polygon->values[i].line.b.x, doom->editor.polygon->values[i].line.b.y}}, 0, 1.5));
	}
	doom->polygons = polygons;
	post_process_polygons(doom);
	set_gui(doom, GUI_INGAME);
}
