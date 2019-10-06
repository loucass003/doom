/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 15:26:39 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/05 17:24:14 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJ_H
# define OBJ_H

# include <libft.h>
# include <SDL.h>
# include "arrays.h"
# include "file_utils.h"
# include "mtl.h"
# include "render.h"

# define OBJ_BUFFER 4096
# define PREFIXES_COUNT 7
# define MTL_PREFIXES_COUNT 3
# define GROUPS_MAX 255
# define GROUPS_NAME_LEN 255

typedef struct	s_reader
{
	uint8_t		buffer[OBJ_BUFFER];
	size_t		pos;
	size_t		len;
	int			fd;
}				t_reader;

typedef struct	s_obj
{
	t_bool			can_add_materials;
	int				current_mtl;
	int				current_group;
	size_t			groups_count;
	char			groups[GROUPS_MAX][GROUPS_NAME_LEN];
	char			*working_dir;
	//TODO: NEED TO BE REPLACED
/* 	t_mtllist		*materials;
	char			*working_dir; //TODO: NEED TO BE SET IN MATERIAL
	t_4dvertices	*vertices;
	t_vec4			*pp_vertices;
	t_vec3			*pp_normals;
	t_faces			*faces;
	t_2dvertices	*vertex;
	t_3dvertices	*normals;
	t_vec3			position;
	t_vec3			rotation;
	t_vec3			scale;
	t_bool			dirty;
	t_bool			fixed; */
}				t_obj;

typedef struct	s_objs
{
	int				len;
	int				capacity;
	t_obj			objs[];
}				t_objs;

typedef struct	s_obj_prefix
{
	char		*prefix;
	t_bool		(*formatter)(t_obj *obj, t_reader *state, t_renderable *r);
}				t_obj_prefix;

char			io_peek(t_reader *r);
void			io_next(t_reader *r);
t_bool			io_expect(t_reader *r, const char *str);
t_bool			io_skip_until(t_reader *r, char *str);
t_bool			io_readfloat(t_reader *r, float *out);
t_bool			io_readnum(t_reader *r, int32_t *out);
void			io_skip_ws(t_reader *r);
void			io_skip_empty(t_reader *r);
t_bool			read_face_part(t_reader *r, size_t i, t_face *face);
t_obj_prefix	*get_formatter(t_obj_prefix *prefixes, size_t prefixes_count, t_reader *reader);
t_bool			face_formatter(t_obj *obj, t_reader *reader, t_renderable *r);
t_bool			vertice_formatter(t_obj *obj, t_reader *reader, t_renderable *r);
t_bool			normal_formatter(t_obj *obj, t_reader *reader, t_renderable *r);
t_bool			vertex_formatter(t_obj *obj, t_reader *reader, t_renderable *r);
t_bool			group_formatter(t_obj *obj, t_reader *reader);
t_bool			mtllib_formatter(t_obj *obj, t_reader *reader, t_renderable *r);
t_bool			usemtl_formatter(t_obj *obj, t_reader *reader, t_renderable *r);
t_bool			mtl_newmtl_formatter(t_obj *obj, t_reader *reader, t_renderable *r);
t_bool			mtl_map_kd_formatter(t_obj *obj, t_reader *reader, t_renderable *r);
t_bool			mtl_kd_formatter(t_obj *o, t_reader *reader, t_renderable *r);
int				get_material(t_renderable *r, char *name, size_t len);
t_bool			free_obj(t_obj *obj, t_bool result);
t_objs			*create_objs_array(int capacity);
t_objs			*append_objs_array(t_objs **arr, t_obj v);
t_objs			*splice_objs_array(t_objs *arr,
						int index, int n);
t_objs			*copy_objs_array(t_objs *src,
						t_objs **dst);
t_bool			post_process_obj(t_renderable *r);
void			transfom_obj(t_obj *obj);

#endif
