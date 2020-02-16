/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 15:26:39 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/16 15:23:08 by lloncham         ###   ########.fr       */
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
# include "io.h"

# define PREFIXES_COUNT 7
# define MTL_PREFIXES_COUNT 3

typedef struct	s_obj
{
	t_bool		can_add_materials;
	int			current_mtl;
	int			current_group;
	char		*working_dir;
}				t_obj;

typedef struct	s_objs
{
	int			len;
	int			capacity;
	t_obj		objs[];
}				t_objs;

typedef struct	s_obj_prefix
{
	char		*prefix;
	t_bool		(*formatter)();
}				t_obj_prefix;

t_bool			read_face_part(t_reader *r, size_t i, t_face *face);
t_obj_prefix	*get_formatter(t_obj_prefix *prefixes, size_t prefixes_count,
					t_reader *reader);
t_bool			face_formatter(t_obj *obj, t_reader *reader, t_renderable *r);
t_bool			vertice_formatter(t_obj *obj, t_reader *reader,
					t_renderable *r);
t_bool			normal_formatter(t_obj *obj, t_reader *reader, t_renderable *r);
t_bool			vertex_formatter(t_obj *obj, t_reader *reader, t_renderable *r);
t_bool			group_formatter(t_obj *obj, t_reader *reader, t_renderable *r);
t_bool			mtllib_formatter(t_obj *obj, t_reader *reader, t_renderable *r);
t_bool			usemtl_formatter(t_obj *obj, t_reader *reader, t_renderable *r);
t_bool			mtl_newmtl_formatter(t_obj *obj, t_reader *reader,
					t_renderable *r);
t_bool			mtl_map_kd_formatter(t_obj *obj, t_reader *reader,
					t_renderable *r);
t_bool			mtl_kd_formatter(t_obj *o, t_reader *reader, t_renderable *r);
int				get_material(t_renderable *r, char *name, size_t len);
t_bool			free_obj(t_obj *obj, t_bool result);
t_objs			*create_objs_array(int capacity);
t_objs			*append_objs_array(t_objs **arr, t_obj v);
t_objs			*splice_objs_array(t_objs *arr, int index, int n);
t_objs			*copy_objs_array(t_objs *src, t_objs **dst);
void			transfom_obj(t_obj *obj);
t_bool			create_obj(struct s_doom *doom, t_renderable *r, char *file);
t_bool			set_obj_working_dir(struct s_doom *doom, char *folder);
t_face			create_face(int indexes[3][3], int material_index,
					int normal_type);

#endif
