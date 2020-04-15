/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressource_model_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/15 03:39:48 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/15 03:40:33 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ressource.h"
#include "doom.h"
#include "obj.h"
#include <libft.h>
#include "write_structs.h"

t_bool		alloc_model(t_renderable *model, t_wr_model wr_model)
{
	if (!(model->faces = create_faces_array(wr_model.faces_count))
		|| !(model->vertices = create_4dvertices_array(wr_model.vertices_count))
		|| !(model->normals = create_3dvertices_array(wr_model.normals_count))
		|| (wr_model.vertex_count > 0
		&& !(model->vertex = create_2dvertices_array(wr_model.vertex_count)))
		|| !(model->materials = create_mtllist(wr_model.materials_count)))
		return (FALSE);
	return (TRUE);
}

t_bool		read_model_arrays(t_ressource_manager *r, t_renderable *model,
	t_wr_model wr_model)
{
	int				i;
	t_mtl			mtl;

	if (!io_memcpy(&r->reader, model->faces->values,
		wr_model.faces_count * sizeof(t_face))
		|| !io_memcpy(&r->reader, model->vertices->vertices,
		wr_model.vertices_count * sizeof(t_vec4))
		|| !io_memcpy(&r->reader, model->normals->vertices,
		wr_model.normals_count * sizeof(t_vec3))
		|| (model->vertex && !io_memcpy(&r->reader, model->vertex->vertices,
		wr_model.vertex_count * sizeof(t_vec2))))
		return (FALSE);
	i = -1;
	while (++i < wr_model.materials_count)
	{
		if (!read_material(r, &mtl))
			return (FALSE);
		if (!append_mtllist(&model->materials, mtl))
			return (FALSE);
	}
	return (TRUE);
}
