/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 15:28:48 by llelievr          #+#    #+#             */
/*   Updated: 2019/12/09 17:09:48 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <libft.h>
#include "obj.h"
#include <doom.h>

t_obj_prefix	*get_formatter(t_obj_prefix *prefixes, size_t prefixes_count, t_reader *reader)
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

t_bool		free_obj(t_obj *obj, t_bool ret)
{
	(void)obj;
	return (ret);
}

t_bool		init_obj(t_doom *doom, t_obj *obj)
{
	obj->current_group = 0;
	obj->groups_count++;
	obj->can_add_materials = TRUE;
	obj->current_mtl = -1;
	obj->working_dir = ft_strdup(doom->obj_working_dir);
	ft_strcpy(obj->groups[0], "root");
	return (TRUE);
}

void		init_prefixes(t_obj_prefix *prefixes)
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

t_bool		load_obj(t_doom *doom, t_renderable *r, t_obj *obj, char *file)
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
	if (!init_obj(doom, obj))
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

t_bool		set_obj_working_dir(t_doom *doom, char *folder)
{
	if (doom->obj_working_dir)
		ft_memdel((void **)&doom->obj_working_dir);
	if (!(doom->obj_working_dir = (char *)ft_strnew(ft_strlen(folder))))
		return (FALSE);
	ft_strcpy(doom->obj_working_dir, folder);
	return (TRUE);
}

t_bool	create_obj(t_doom *doom, t_renderable *r, char *file)
{
	ft_bzero(r, sizeof(t_renderable));
	r->of.type = RENDERABLE_OBJ;
	if (!(r->of.data.obj = malloc(sizeof(t_obj))))
		return (FALSE);
	if(!(r->vertices = create_4dvertices_array(800)))
		return (free_renderable(&r, FALSE));
	if(!(r->vertex = create_2dvertices_array(800)))
		return (free_renderable(&r, FALSE));
	if(!(r->normals = create_3dvertices_array(800)))
		return (free_renderable(&r, FALSE));
	if(!(r->faces = create_faces_array(800)))
		return (free_renderable(&r, FALSE));
	if(!(r->materials = create_mtllist(3)))
		return (free_renderable(&r, FALSE));
	if (!load_obj(doom, r, r->of.data.obj, file))
		return (free_renderable(&r, FALSE));
	if (r->materials->len == 0)
	{
		printf("ADD DEFAULT MATERIAL\n");
		if (!append_mtllist(&r->materials, (t_mtl){ .material_color_set = TRUE, .material_color = 0xFFFFFFFF }))
			return (FALSE);
	}
	if (r->normals->len == 0)
		return (FALSE);
	post_process_renderable(doom, r, TRUE);
	r->scale = (t_vec3){ 1, 1, 1 };
	r->dirty = TRUE;
	return (TRUE);
}

t_bool		obj_test(t_doom *doom)
{
	/* t_bool lol = load_obj(doom, &obj, "House2.obj");
	obj->fixed = TRUE;
	obj->position = (t_vec3){1, 0, 4};
	obj->scale = (t_vec3){0.003, 0.003, 0.003}; */
	//append_objs_array(&doom->objects, (t_obj){ .renderable = NULL });
	//t_bool lol = load_obj(doom, &doom->objects->objs[doom->objects->len - 1], "House2.obj");
	 t_renderable r;
	t_bool lol;
	
	


	set_obj_working_dir(doom, "assets/obj");
	lol = create_obj(doom, &r, "skybox.obj");
	r.position = (t_vec3){0, 0, 0};
	r.scale = (t_vec3){FAR_CULL, FAR_CULL, FAR_CULL};
	r.no_light = TRUE;
	r.fixed = TRUE;
	doom->skybox_index = doom->renderables->len;
	append_renderables_array(&doom->renderables, r);
	// set_obj_working_dir(doom, "assets/obj/de_dust");
	// lol = create_obj(doom, &r, "de_dust2.obj");
	// r.position = (t_vec3){0, 0, 0};
	// r.rotation = (t_vec3){0, 0, 0};
	// r.scale = (t_vec3){0.05, 0.05, 0.05};
	// //r.wireframe = TRUE;
	// r.wireframe_color = 0xFFFF0000;
	// r.fixed = TRUE;
	// append_renderables_array(&doom->renderables, r);
	set_obj_working_dir(doom, "assets/obj/winter");
	lol = create_obj(doom, &r, "winter.obj");
	r.position = (t_vec3){0, 0, -3};
	r.rotation = (t_vec3){0, 0, 0};
//	r.scale = (t_vec3){0.05, 0.05, 0.05};
	r.scale = (t_vec3){5, 5, 5};
	//r.wireframe = TRUE;
	r.wireframe_color = 0xFFFF0000;
	r.fixed = TRUE;
	append_renderables_array(&doom->renderables, r);
// 	set_obj_working_dir(doom, "assets/obj");
// 	lol = create_obj(doom, &r, "polarbear.obj");
// 	r.position = (t_vec3){0, 0, 0};
// 	r.rotation = (t_vec3){0, 0, 0};
// //	r.scale = (t_vec3){0.05, 0.05, 0.05};
// 	r.scale = (t_vec3){1, 1, 1};
// 	//r.wireframe = TRUE;
// 	r.wireframe_color = 0xFFFF0000;
// 	r.fixed = TRUE;
// 	append_renderables_array(&doom->renderables, r);
	// set_obj_working_dir(doom, "assets/obj");
	// lol = create_obj(doom, &doom->grenada_primitive, "grenada.obj");
	// doom->grenada_primitive.position = (t_vec3){0, 0, 0};
	// doom->grenada_primitive.rotation = (t_vec3){0, 10, 0};
	// doom->grenada_primitive.scale = (t_vec3){0.05, 0.05, 0.05};
	//append_renderables_array(&doom->renderables, doom->grenada_primitive);
//	post_process_obj(doom, &doom->grenada_primitive);
	//r.wireframe = TRUE;
	//r.wireframe_color = 0xFFFF0000;
	//r.fixed = TRUE;
	// set_obj_working_dir(doom, "assets/obj/cs_italy");
	// lol = create_obj(doom, &r, "cs_italy.obj");
	// r.position = (t_vec3){0, 0, 0};
	// r.scale = (t_vec3){0.06, 0.06, 0.06};
	// r.fixed = TRUE;
	// append_renderables_array(&doom->renderables, r);
	set_obj_working_dir(doom, "assets/obj");
	lol = create_obj(doom, &r, "House2.obj");
	r.position = (t_vec3){-10, 0, 0};
	r.scale = (t_vec3){0.03, 0.03, 0.03};
	r.fixed = TRUE;
	append_renderables_array(&doom->renderables, r);
	
	// lol = create_obj(doom, &r, "skybox.obj");
	// r.position = (t_vec3){0, 0, 0};
	// r.scale = (t_vec3){FAR_CULL, FAR_CULL, FAR_CULL};
	// r.no_light = TRUE;
	// r.double_faced = TRUE;
	// //r.fixed = TRUE;
	// append_renderables_array(&doom->renderables, r);
	
	printf("ERROR %d\n", !lol);
	return (FALSE);
}
