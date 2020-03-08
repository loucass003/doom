/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressource_model.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 10:50:33 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/08 20:46:00 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "ressource.h"
#include "obj.h"
#include "write_structs.h"

t_bool		load_model_file(t_doom *doom, t_ressource *r, char *path)
{
	t_renderable	*model;
	char			*folder;

	folder = ft_strndup(path, ft_strrchr(path, '/') - path);
	set_obj_working_dir(doom, folder);
	free(folder);
	if (!(model = malloc(sizeof(t_renderable))))
		return (FALSE);
	if (!create_obj(doom, model, ft_strrchr(path, '/')))
		return (FALSE);
	r->loaded = TRUE;
	r->data.model = model;
	return (TRUE);
}

t_bool		write_material(t_ressource_manager *r, t_mtl *material)
{
	t_wr_mtl	wr_mtl;

	wr_mtl = (t_wr_mtl) {
		.texture_map_set = material->texture_map_set,
		.material_color_set = material->material_color_set,
		.transparent = material->transparent,
		.wireframe = material->wireframe,
		.material_color = material->material_color
	};
	dp_write(r, &wr_mtl, sizeof(t_wr_mtl));
	if (material->texture_map_set && material->texture_map)
		write_texture(r, material->texture_map);
	return (TRUE);
}

t_bool		read_material(t_ressource_manager *r, t_mtl *mtl)
{
	t_wr_mtl	wr_mtl;

	if (!io_memcpy(&r->reader, &wr_mtl, sizeof(t_wr_mtl)))
		return (FALSE);
	mtl->texture_map_set = wr_mtl.texture_map_set;
	mtl->material_color_set = wr_mtl.material_color_set;
	mtl->material_color = wr_mtl.material_color;
	mtl->wireframe = wr_mtl.wireframe;
	mtl->transparent = wr_mtl.transparent;
	if (wr_mtl.texture_map_set && !read_texture(r, &mtl->texture_map))
		return (FALSE);
	return (TRUE);
}

t_bool		write_model(t_ressource_manager *r, t_renderable *model)
{
	int			i;
	t_wr_model	wr_model;

	wr_model = (t_wr_model) {
		.vertices_count = model->vertices->len,
		.vertex_count = model->vertex ? model->vertex->len : 0,
		.normals_count = model->normals->len,
		.materials_count = model->materials->len,
		.faces_count = model->faces->len,
		.groups_count = model->groups_count};
	ft_memcpy(wr_model.groups, model->groups, GROUPS_NAME_LEN * GROUPS_MAX);
	dp_write(r, &wr_model, sizeof(t_wr_model));
	dp_write(r, model->faces->values, sizeof(t_face) * model->faces->len);
	dp_write(r, model->vertices->vertices, sizeof(t_vec4)
		* model->vertices->len);
	dp_write(r, model->normals->vertices, sizeof(t_vec3) * model->normals->len);
	if (model->vertex)
		dp_write(r, model->vertex->vertices, sizeof(t_vec2)
			* model->vertex->len);
	i = -1;
	while (++i < model->materials->len)
		if (!write_material(r, &model->materials->values[i]))
			return (FALSE);
	return (TRUE);
}

t_bool		read_model(t_ressource_manager *r, t_renderable **m)
{
	t_renderable	*model;
	t_wr_model		wr_model;
	t_mtl			mtl;
	int				i;

	if (!(model = ft_memalloc(sizeof(t_renderable))))
		return (FALSE);
	if (!io_memcpy(&r->reader, &wr_model, sizeof(t_wr_model)))
		return (FALSE);
	if (!(model->faces = create_faces_array(wr_model.faces_count))
		|| !(model->vertices = create_4dvertices_array(wr_model.vertices_count))
		|| !(model->normals = create_3dvertices_array(wr_model.normals_count))
		|| (wr_model.vertex_count > 0
		&& !(model->vertex = create_2dvertices_array(wr_model.vertex_count)))
		|| !(model->materials = create_mtllist(wr_model.materials_count)))
		return (FALSE);
	model->faces->len = wr_model.faces_count;
	model->vertices->len = wr_model.vertices_count;
	model->normals->len = wr_model.normals_count;
	if (model->vertex)
		model->vertex->len = wr_model.vertex_count;
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
	model->fixed = TRUE;
	model->scale = (t_vec3){ 1, 1, 1 };
	model->visible = TRUE;
	model->object_index = -1;
	model->groups_count = wr_model.groups_count;
	ft_memcpy(model->groups, wr_model.groups, GROUPS_NAME_LEN * GROUPS_MAX);
	post_process_renderable(r->doom, model, TRUE, FALSE);
	*m = model;
	return (TRUE);
}
