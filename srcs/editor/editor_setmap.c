/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_setmap.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 13:54:34 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/09 11:15:45 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "light.h"

void set_map(t_doom *doom)
{
	/* t_polygons *polygons = create_polygons_array(doom->editor.polygon->len + 2);
	
	t_3dvertices *vertices = create_3dvertices_array(doom->editor.polygon->len);
	for (int i = 0; i < doom->editor.polygon->len; i++)
	{
		append_3dvertices_array(&vertices, (t_vec3){
			doom->editor.polygon->values[doom->editor.polygon->len - 1 - i].line.a.x, 0, doom->editor.polygon->values[doom->editor.polygon->len - 1 - i].line.a.y});
	}
	append_polygons_array(&polygons, create_polygon(vertices, P_FLOOR, doom->textures.bricks));
	
	t_3dvertices *vertices_ceil = create_3dvertices_array(doom->editor.polygon->len);
	for (int i = 0; i < doom->editor.polygon->len; i++)
	{
		append_3dvertices_array(&vertices_ceil, (t_vec3){
			doom->editor.polygon->values[i].line.a.x, 1.5, doom->editor.polygon->values[i].line.a.y});
	}
	append_polygons_array(&polygons, create_polygon(vertices_ceil, P_CEILING, doom->textures.bricks));
	
	for (int i = 0; i < doom->editor.polygon->len; i++)
	{
		append_polygons_array(&polygons, create_wall_polygon(doom->editor.polygon->values[i].img, (t_line){ (t_vec2){doom->editor.polygon->values[i].line.a.x, (doom->editor.polygon->values[i].line.a.y)}, (t_vec2){doom->editor.polygon->values[i].line.b.x, doom->editor.polygon->values[i].line.b.y}}, 0, 1.5));
	} */
	/* doom->polygons = polygons;
	post_process_map(doom);
	set_gui(doom, GUI_INGAME); */

	splice_renderables_array(doom->renderables, 0, doom->renderables->len);

	t_renderable floor;
	create_polygon(&floor, P_FLOOR, (t_mtl){ .texture_map = doom->textures.bricks, .texture_map_set = TRUE });
	for (int i = 0; i < doom->editor.polygon->len; i++)
	{
		add_polygon_vertice(&floor, (t_vec4){doom->editor.polygon->values[i].line.a.x, 0, doom->editor.polygon->values[i].line.a.y, 1});
	}
	append_renderables_array(&doom->renderables, floor);
	
	t_renderable ceiling;
	create_polygon(&ceiling, P_CEILING, (t_mtl){ .texture_map = doom->textures.bricks, .texture_map_set = TRUE });
	for (int i = 0; i < doom->editor.polygon->len; i++)
	{
		add_polygon_vertice(&ceiling, (t_vec4){ doom->editor.polygon->values[doom->editor.polygon->len - 1 - i].line.a.x, 1.5, doom->editor.polygon->values[doom->editor.polygon->len - 1 - i].line.a.y, 1});
	}
	
	append_renderables_array(&doom->renderables, ceiling);
	
	for (int i = 0; i < doom->editor.polygon->len; i++)
	{
		t_renderable wall;
		create_wall_polygon(&wall, (t_mtl){ .texture_map = doom->textures.bricks, .texture_map_set = TRUE }, (t_line){ (t_vec2){doom->editor.polygon->values[i].line.a.x, (doom->editor.polygon->values[i].line.a.y)}, (t_vec2){doom->editor.polygon->values[i].line.b.x, (doom->editor.polygon->values[i].line.b.y)}}, (t_vec2){0, 1.5});
		append_renderables_array(&doom->renderables, wall);
	}
	post_process_map(doom);
	set_gui(doom, GUI_INGAME);
}
