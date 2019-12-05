/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube_renderable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/05 14:34:42 by llelievr          #+#    #+#             */
/*   Updated: 2019/12/05 15:46:30 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "render.h"

t_bool	gen_cube_mesh(t_renderable *r, t_bool inside)
{
	append_4dvertices_array(&r->vertices, (t_vec4){ 0.5, 0.5, -0.5, 1 });
	append_4dvertices_array(&r->vertices, (t_vec4){ 0.5, -0.5, -0.5, 1 });
	append_4dvertices_array(&r->vertices, (t_vec4){ 0.5, 0.5, 0.5, 1 });
	append_4dvertices_array(&r->vertices, (t_vec4){ 0.5, -0.5, 0.5, 1 });
	append_4dvertices_array(&r->vertices, (t_vec4){ -0.5, 0.5, -0.5, 1 });
	append_4dvertices_array(&r->vertices, (t_vec4){ -0.5, -0.5, -0.5, 1 });
	append_4dvertices_array(&r->vertices, (t_vec4){ -0.5, 0.5, 0.5, 1 });
	append_4dvertices_array(&r->vertices, (t_vec4){ -0.5, -0.5, 0.5, 1 });

	append_2dvertices_array(&r->vertex, (t_vec2){ 0.749957, 0.749957 });
	append_2dvertices_array(&r->vertex, (t_vec2){ 0.500000, 0.500000 });
	append_2dvertices_array(&r->vertex, (t_vec2){ 0.500000, 0.749957 });
	append_2dvertices_array(&r->vertex, (t_vec2){ 0.250043, 0.250043 });
	append_2dvertices_array(&r->vertex, (t_vec2){ 0.500000, 0.000087 });
	append_2dvertices_array(&r->vertex, (t_vec2){ 0.250044, 0.000087 });
	append_2dvertices_array(&r->vertex, (t_vec2){ 0.250043, 0.500000 });
	append_2dvertices_array(&r->vertex, (t_vec2){ 0.500000, 0.250043 });

	append_3dvertices_array(&r->normals, (t_vec3){ 0, 1, 0 });
	append_3dvertices_array(&r->normals, (t_vec3){ 0, 0, 1 });
	append_3dvertices_array(&r->normals, (t_vec3){ -1, 0, 0 });
	append_3dvertices_array(&r->normals, (t_vec3){ 0, -1, 0 });
	append_3dvertices_array(&r->normals, (t_vec3){ 1, 0, 0 });
	append_3dvertices_array(&r->normals, (t_vec3){ 0, 0, -1 });

	int     faces[12][3][3] = {
		{{ 3, 5, 1 }, { 1, 2, 3 }, { 1, 1, 1 }},
		{{ 8, 3, 4 }, { 4, 5, 6 }, { 2, 2, 2 }},
		{{ 6, 7, 8 }, { 7, 8, 4 }, { 3, 3, 3 }},
		{{ 8, 2, 6 }, { 9, 10, 7 }, { 4, 4, 4 }},
		{{ 4, 1, 2 }, { 11, 3, 10 }, { 5, 5, 5 }},
		{{ 2, 5, 6 }, { 10, 2, 7 }, { 6, 6, 6 }},
		{{ 3, 7, 5 }, { 1, 12, 2 }, { 1, 1, 1 }},
		{{ 8, 7, 3 }, { 4, 8, 5 }, { 2, 2, 2 }},
		{{ 6, 5, 7 }, { 7, 2, 8 }, { 3, 3, 3 }},
		{{ 8, 4, 2 }, { 9, 13, 10 }, { 4, 4, 4 }},
		{{ 4, 3, 1 }, { 11, 14, 3 }, { 5, 5, 5 }},
		{{ 2, 1, 5 }, { 10, 3, 2 }, { 6, 6, 6 }},
	};
	
	int     i;

	i = -1;
	while (++i < 12)
		if (!append_faces_array(&r->faces, create_face(faces[i], 0, inside ? 1 : 0)))
			return (FALSE);
	return (TRUE);
}

t_bool	create_cube(t_doom *doom, t_renderable *r, t_bool inside)
{
	if (!create_renderable(r, RENDERABLE_UNKNOWN))
		return (FALSE);
	if (!append_mtllist(&r->materials, (t_mtl){
			.material_color_set = TRUE, .material_color = 0xFFFF0000 }))
		return (free_renderable(&r, FALSE));
	gen_cube_mesh(r, inside);
	post_process_renderable(doom, r, TRUE);
	r->dirty = TRUE;
	r->fixed = FALSE; 
	r->scale = (t_vec3){ 1, 1, 1 };
	r->object_index = -1;
	//r->double_faced = TRUE;
	return (TRUE);
}
