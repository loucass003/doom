/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 15:28:48 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/09 03:12:24 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <libft.h>
#include "obj.h"
#include <doom.h>

t_obj_prefix	*get_formatter(t_obj_prefix *prefixes, size_t prefixes_count,
	t_reader *reader)
{
	char		name[40];
	char		c;
	size_t		len;
	size_t		i;

	io_skip_ws(reader);
	len = 0;
	ft_bzero(name, sizeof(char) * 40);
	while (len < 40 && io_peek(reader, &c) && c != ' ' && c != '\n')
	{
		io_next(reader);
		name[len++] = c;
	}
	if (c != ' ')
		return (NULL);
	i = 0;
	while (i < prefixes_count)
	{
		if (ft_strlen(prefixes[i].prefix) == len
			&& ft_strncmp(prefixes[i].prefix, name, len) == 0)
			return (prefixes + i);
		i++;
	}
	return (prefixes + prefixes_count);
}

t_bool			free_obj(t_obj *obj, t_bool ret)
{
	(void)obj;
	return (ret);
}

t_bool			init_obj(t_doom *doom, t_obj *obj, t_renderable *r)
{
	obj->current_group = 0;
	r->groups_count = 1;
	obj->can_add_materials = TRUE;
	obj->current_mtl = -1;
	obj->working_dir = ft_strdup(doom->obj_working_dir);
	ft_strcpy(r->groups[0], "root");
	return (TRUE);
}

void			init_prefixes(t_obj_prefix *prefixes)
{
	prefixes[0] = (t_obj_prefix){ "v", vertice_formatter };
	prefixes[1] = (t_obj_prefix){ "f", face_formatter };
	prefixes[2] = (t_obj_prefix){ "vn", normal_formatter };
	prefixes[3] = (t_obj_prefix){ "vt", vertex_formatter };
	prefixes[4] = (t_obj_prefix){ "g", group_formatter };
	prefixes[5] = (t_obj_prefix){ "mtllib", mtllib_formatter };
	prefixes[6] = (t_obj_prefix){ "usemtl", usemtl_formatter };
	prefixes[PREFIXES_COUNT] = (t_obj_prefix){ NULL, NULL };
}

t_bool			load_obj(t_doom *doom, t_renderable *r, t_obj *obj, char *file)
{
	t_reader		reader;
	t_obj_prefix	prefixes[PREFIXES_COUNT + 1];
	t_obj_prefix	*formatter;
	char			*path;

	init_prefixes(prefixes);
	ft_bzero(&reader, sizeof(t_reader));
	if (!(path = path_join(doom->obj_working_dir, file))
		|| (reader.fd = open(path, O_RDONLY)) == -1)
		return (FALSE);
	free(path);
	if (!init_obj(doom, obj, r))
		return (FALSE);
	while (!!(formatter = get_formatter(prefixes, PREFIXES_COUNT, &reader)))
	{
		if (formatter->formatter && !formatter->formatter(obj, &reader, r))
			return (free_obj(obj, FALSE));
		io_skip_until(&reader, "\n");
		io_skip_empty(&reader);
	}
	if (!formatter && io_peek(&reader, NULL))
		return (FALSE);
	return (TRUE);
}

t_bool			set_obj_working_dir(t_doom *doom, char *folder)
{
	if (doom->obj_working_dir)
		ft_memdel((void **)&doom->obj_working_dir);
	if (!(doom->obj_working_dir = (char *)ft_strnew(ft_strlen(folder))))
		return (FALSE);
	ft_strcpy(doom->obj_working_dir, folder);
	return (TRUE);
}

t_bool			create_obj(t_doom *doom, t_renderable *r, char *file)
{
	t_obj		obj;

	ft_bzero(r, sizeof(t_renderable));
	r->of.type = RENDERABLE_UNKNOWN;
	if (!(r->vertices = create_4dvertices_array(800)))
		return (free_renderable(r, FALSE, TRUE, FALSE));
	if (!(r->vertex = create_2dvertices_array(800)))
		return (free_renderable(r, FALSE, TRUE, FALSE));
	if (!(r->normals = create_3dvertices_array(800)))
		return (free_renderable(r, FALSE, TRUE, FALSE));
	if (!(r->faces = create_faces_array(800)))
		return (free_renderable(r, FALSE, TRUE, FALSE));
	if (!(r->materials = create_mtllist(3)))
		return (free_renderable(r, FALSE, TRUE, FALSE));
	if (!load_obj(doom, r, &obj, file))
		return (free_renderable(r, FALSE, TRUE, FALSE));
	if (r->materials->len == 0)
	{
		if (!append_mtllist(&r->materials, (t_mtl){
			.material_color_set = TRUE,
			.material_color = 0xFFFFFFFF }))
			return (FALSE);
	}
	if (r->normals->len == 0)
		return (FALSE);
	post_process_renderable(doom, r, TRUE, FALSE);
	r->scale = (t_vec3){ 1, 1, 1 };
	r->dirty = TRUE;
	r->visible = TRUE;
	free(obj.working_dir);
	return (TRUE);
}
