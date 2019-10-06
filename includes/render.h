/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 17:16:19 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/06 03:37:05 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include "image.h"
# include "camera.h"
# include "maths/triangle.h"
# include "mtl.h"
# include "arrays.h"
# include "renderable_of.h"
# include "collision.h"

typedef struct		s_face
{
	int32_t			vertices_index[3];
	t_bool			vertex_set;
	int32_t			vertex_index[3];
	t_bool			normals_set;
	int32_t			normals_index[3];
	t_vec3			face_normal;
	t_mtl			*mtl;
	t_collidable	collidable;
	t_bool			selected;
	t_bool			hidden;
	t_bool			already_collided;
	int				group;
}					t_face;

typedef struct		s_faces
{
	int				len;
	int				capacity;
	t_face			values[];
}					t_faces;

typedef enum		s_render_type
{
	CTX_NORMAL,
}					t_render_type;

typedef struct		s_render_data
{
	t_triangle		triangle;
	t_mtl			*mtl;
}					t_render_data;

typedef struct		s_render_context
{
	t_render_type	type;
	t_camera		*camera;
	float			*buffer;
	t_img			*image;
	struct s_doom	*doom;
}					t_render_context;

typedef struct		s_renderable
{
	t_renderable_of			of;
	t_4dvertices			*vertices;
	t_vec4					*pp_vertices;
	t_vec3					*pp_normals;
	t_faces					*faces;
	t_2dvertices			*vertex;
	t_3dvertices			*normals;
	t_mtllist				*materials;
	struct s_entity			*entity;
	t_vec3					position;
	t_vec3					rotation;
	t_vec3					scale;
	t_bool					visible;
	t_bool					dirty;
	t_bool					fixed;
	t_bool					double_faced;
	t_bool					no_light;
	struct s_octree_node	*octree;
}					t_renderable;

typedef struct		s_renderables
{
	int				len;
	int				capacity;
	t_renderable	values[];
}					t_renderables;

t_faces				*create_faces_array(int capacity);
t_faces				*append_faces_array(t_faces **arr, t_face i);

t_renderables		*create_renderables_array(int capacity);
t_renderables		*append_renderables_array(t_renderables **arr, t_renderable r);
t_renderables		*splice_renderables_array(t_renderables *arr,
						int index, int n);
t_renderables		*copy_renderables_array(t_renderables *src,
						t_renderables **dst);

t_bool				free_renderable(t_renderable **r, t_bool res);
void				draw_triangle(t_render_context *ctx, t_render_data data);
void				process_triangle(t_render_context *ctx, t_mtl *mtl, t_triangle triangle);
void				post_process_triangle(t_render_context *ctx, t_mtl *mtl, t_triangle triangle);
void				transform_renderable(t_renderable *r);
// void				render_polygon(t_render_context *ctx, t_polygon *poly);
// void				render_obj(t_render_context *ctx, t_obj *obj);

t_collidable		compute_collidable(t_renderable *r, int face_index, t_vec4 *vertices);
t_collidable		face_collidable(t_renderable *r, int face_index, t_vec4 *vertices);

#endif
