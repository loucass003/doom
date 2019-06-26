/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 15:28:48 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/26 17:09:15 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "obj.h"

t_bool		free_obj(t_obj *obj, t_bool ret)
{
	return (ret);
}

t_bool		init_obj(t_obj *obj)
{
	if(!(obj->vertices = create_4dvertices_array(800)))
		return (free_obj(obj, FALSE));
	if(!(obj->vertex = create_2dvertices_array(800)))
		return (free_obj(obj, FALSE));
	if(!(obj->normals = create_3dvertices_array(800)))
		return (free_obj(obj, FALSE));
	if(!(obj->faces = create_faces_array(800)))
		return (free_obj(obj, FALSE));
	return (TRUE);
}

t_bool		load_obj(t_obj *obj, char *file)
{
	t_reader		reader;
	t_obj_prefix	prefixes[PREFIXES_COUNT];
	t_obj_prefix	*formatter;
	size_t			i;

	prefixes[0] = (t_obj_prefix){ "v", vertice_formatter };
	prefixes[1] = (t_obj_prefix){ "f", face_formatter };
	prefixes[2] = (t_obj_prefix){ "vn", normal_formatter };
	prefixes[3] = (t_obj_prefix){ "vt", vertex_formatter };
	prefixes[4] = (t_obj_prefix){ "g", group_formatter };
	prefixes[5] = (t_obj_prefix){ "mtllib", mtllib_formatter };
	prefixes[6] = (t_obj_prefix){ "usemtl", usemtl_formatter };
	prefixes[7] = (t_obj_prefix){ "#", NULL };
	ft_bzero(&reader, sizeof(t_reader));
	if ((reader.fd = open(file, O_RDONLY)) == -1)
		return (FALSE);
	init_obj(obj);
	i = 0;
	while ((formatter = get_formatter(prefixes, &reader)) != NULL)
	{
		if (formatter->formatter && !formatter->formatter(obj, &reader))
			return (free_obj(obj, FALSE));
		io_skip_until(&reader, "\n");
		io_skip_empty(&reader);
	}
	if (!formatter && io_peek(&reader) != -1)
		return (FALSE);
	return (TRUE);
}

t_bool		obj_test()
{
	t_obj obj;
	t_bool lol = load_obj(&obj, "assets/obj/cow-normals.obj");
	printf("ERROR %d\n", !lol);
	return (FALSE);
}
