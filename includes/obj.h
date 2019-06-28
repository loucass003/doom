/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 15:26:39 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/28 03:27:35 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJ_H
# define OBJ_H

# define OBJ_BUFFER 200
# define PREFIXES_COUNT 7
# define MTL_PREFIXES_COUNT 2
# define GROUPS_MAX 255
# define GROUPS_NAME_LEN 255
# define MATERIAL_FILE_LEN 255
# define MATERIAL_NAME_LEN 255
# include "doom.h" 

typedef struct	s_mtl
{
	char		name[MATERIAL_NAME_LEN];
	SDL_Surface	*texture_map;	
}				t_mtl;

typedef struct	s_face
{
	int32_t			vertices_index[3];
	t_bool			vertex_set;
	int32_t			vertex_index[3];
	t_bool			normals_set;
	int32_t			normals_index[3];
	t_mtl			*mtl;
}				t_face;

typedef struct	s_faces
{
	int				len;
	int				capacity;
	t_face			values[];
}				t_faces;

typedef struct	s_mtllist
{
	int				len;
	int				capacity;
	t_mtl			values[];
}				t_mtllist;

typedef struct	s_reader
{
	uint8_t		buffer[OBJ_BUFFER];
	size_t		pos;
	size_t		len;
	int			fd;
}				t_reader;

typedef struct	s_obj
{
	t_4dvertices	*vertices;
	t_faces			*faces;
	t_2dvertices	*vertex;
	t_3dvertices	*normals;
	t_bool			can_add_materials;
	t_mtllist		*materials;
	t_mtl			*current_mtl;
	int				current_group;
	size_t			groups_count;
	char			groups[GROUPS_MAX][GROUPS_NAME_LEN];
	char			*working_dir;
}				t_obj;

typedef struct	s_obj_prefix
{
	char		*prefix;
	t_bool		(*formatter)(t_obj *obj, t_reader *state);
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
t_faces			*create_faces_array(int capacity);
t_faces			*append_faces_array(t_faces **arr, t_face i);
t_mtllist		*create_mtllist(int capacity);
t_mtllist		*append_mtllist(t_mtllist **arr, t_mtl v);
t_obj_prefix	*get_formatter(t_obj_prefix *prefixes, size_t prefixes_count, t_reader *reader);
t_bool			face_formatter(t_obj *obj, t_reader *reader);
t_bool			vertice_formatter(t_obj *obj, t_reader *reader);
t_bool			normal_formatter(t_obj *obj, t_reader *reader);
t_bool			vertex_formatter(t_obj *obj, t_reader *reader);
t_bool			group_formatter(t_obj *obj, t_reader *reader);
t_bool			mtllib_formatter(t_obj *obj, t_reader *reader);
t_bool			usemtl_formatter(t_obj *obj, t_reader *reader);
t_bool			mtl_newmtl_formatter(t_obj *obj, t_reader *reader);
t_bool			mtl_map_kd_formatter(t_obj *obj, t_reader *reader);

#endif
