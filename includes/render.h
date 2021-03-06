/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 17:16:19 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/20 19:21:47 by Lisa             ###   ########.fr       */
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

# define GROUPS_MAX 255
# define GROUPS_NAME_LEN 255
# define EPSILON 1e-6

typedef struct				s_face
{
	int32_t					vertices_index[3];
	t_bool					vertex_set;
	int32_t					vertex_index[3];
	t_bool					normals_set;
	int32_t					normals_index[3];
	t_vec3					face_normal;
	int						mtl_index;
	t_collidable			collidable;
	t_bool					hidden;
	t_bool					double_sided;
	t_bool					has_collision;
	char					normal_type;
	int						group;
	t_bool					rendered;
	int						wall_index;
	int						wall_section;
	int						room_index;
}							t_face;

typedef struct				s_faces
{
	int						len;
	int						capacity;
	t_face					values[];
}							t_faces;

typedef enum				e_render_type
{
	CTX_NORMAL,
	CTX_EDITOR
}							t_render_type;

typedef struct				s_render_context
{
	t_render_type			type;
	t_camera				*camera;
	float					*buffer;
	t_img					*image;
	struct s_doom			*doom;
}							t_render_context;

typedef struct				s_render_data
{
	t_triangle				triangle;
	t_mtl					*mtl;
	t_render_context		*ctx;
	t_vec2					min;
	t_vec2					max;
}							t_render_data;

typedef struct				s_render_datas
{
	int						len;
	int						capacity;
	t_render_data			values[];
}							t_render_datas;

typedef struct				s_renderable
{
	t_renderable_of			of;
	t_4dvertices			*vertices;
	t_vec4					*pp_vertices;
	t_vec3					*pp_normals;
	t_faces					*faces;
	t_2dvertices			*vertex;
	t_3dvertices			*normals;
	t_mtllist				*materials;
	struct s_sprite			*sprite;
	struct s_octree_node	*octree;
	t_vec3					position;
	t_vec3					rotation;
	t_bool					no_collision;
	t_vec3					scale;
	t_mat4					matrix;
	t_bool					rot_matrix_set;
	t_mat4					rot_matrix;
	t_bool					visible;
	t_bool					dirty;
	t_bool					fixed;
	t_bool					double_faced;
	t_bool					no_light;
	t_bool					show_hitbox;
	t_bool					has_hitbox;
	t_bool					model;
	t_collidable			hitbox;
	t_vec3					hitbox_offset;
	t_bool					wireframe;
	int						wireframe_color;
	int						object_index;
	size_t					groups_count;
	char					groups[GROUPS_MAX][GROUPS_NAME_LEN];
}							t_renderable;

typedef struct				s_renderables
{
	int						len;
	int						capacity;
	t_renderable			values[];
}							t_renderables;

typedef struct				s_rgba
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
}							t_rgba;

typedef union				u_color_r
{
	uint32_t	color;
	t_rgba		argb;
}							t_color_r;

typedef struct				s_pixel_data
{
	int			x;
	int			y;
	float		t;
	t_vertex	start;
	t_vertex	end;
}							t_pixel_data;

typedef struct				s_face_data
{
	t_render_context	*ctx;
	t_renderable		*r;
}							t_face_data;

typedef struct				s_triangulate
{
	t_vec3					n;
	int						*filter;
	int						filter_len;
	int						normal_type;
	int						mtl;
	int						room_index;
	t_vec2					offset;
	t_vec2					repeat;
	struct s_4dvertices		*vertices;
}							t_triangulate;

t_faces						*create_faces_array(int capacity);
t_faces						*append_faces_array(t_faces **arr, t_face i);

t_renderables				*create_renderables_array(int capacity);
t_renderables				*append_renderables_array(t_renderables **arr,
								t_renderable r);
t_renderables				*splice_renderables_array(t_renderables *arr,
								int index, int n);
t_renderables				*copy_renderables_array(t_renderables *src,
								t_renderables **dst);
t_bool						copy_renderable(t_renderable *src,
								t_renderable *dest);
int							renderables_indexof(t_renderables *arr,
								t_renderable *elem);

t_render_datas				*create_render_data_array(int capacity);
t_render_datas				*append_render_data_array(t_render_datas **arr,
								t_render_data r);
t_render_datas				*splice_render_data_array(t_render_datas *arr,
								int index, int n);
t_render_datas				*copy_render_data_array(t_render_datas *src,
								t_render_datas **dst);
int							render_data_indexof(t_render_datas *arr,
								t_render_data *elem);

void						draw_triangle(t_render_data data);
void						process_triangle(t_render_context *ctx, t_mtl *mtl,
								t_triangle triangle);
void						post_process_triangle(t_render_context *ctx,
								t_mtl *mtl, t_triangle triangle);
void						transform_renderable(t_renderable *r);
t_bool						post_process_renderable(struct s_doom *doom,
								t_renderable *r, t_bool octree, t_bool replace);

float						get_lights_intensity(t_face_data *fd,
								t_vec3 normal, t_vec4 point, t_face *face);

t_collidable				compute_collidable(t_renderable *r, int face_index,
								t_vec4 *vertices);
t_collidable				face_collidable(t_renderable *r, int face_index,
								t_vec4 *vertices);
void						compute_ellipsoid_hitbox(t_renderable *r,
								t_vec3 pos, t_vec3 radius);
void						update_hitbox(t_renderable *r);
t_collision					ray_hit_world(struct s_doom *doom,
								t_renderables *renderables, t_ray ray);
t_bool						create_renderable(t_renderable	*r,
								t_renderable_type type);
t_bool						create_cube(struct s_doom *doom, t_renderable *r,
								t_bool inside);
void						compute_collidables(t_renderable *r);
void						draw_line_zbuff(t_render_context *ctx, t_vec4 v0,
								t_vec4 v1, int color);
t_bool						render_renderable(t_render_context *ctx,
								t_renderable *r);
t_bool						triangulate_floor_ceil(t_renderable *r,
								t_triangulate *f_c);
t_bool						compute_change_of_basis(t_vec3 n, t_mat4 *p_inv,
								t_mat4 *reverse);
t_bool						ear_clip2(t_triangulate *t, t_faces **faces);
t_bool						inside_triangle(t_vec4 a, t_vec4 b, t_vec4 c,
								t_vec4 p);
t_bool						free_ret(void *value, t_bool ret);
float						area(t_4dvertices *vertices,
								int *filters, int filters_count);
void						uv_mapping(t_triangulate *t, t_2dvertices *vertex);

void						free_materials(t_mtllist **m_addr,
								t_bool free_ressources);
void						free_renderable_of(t_renderable *r);
t_bool						free_renderable(t_renderable *r,
								t_bool free_resources, t_bool res);
void						face_and_triangle(t_renderable *r, int i);
void						free_renderables(t_renderables **renderables,
								t_bool destroy_array);
void						draw_pixel(t_render_data data, t_pixel_data pd);
void						render_face(int face_index, void *p);
void						prepare_points_light(float it[3],
								t_face *face, t_face_data *fd);
void						prepare_triangle(t_face_data *fd, t_face *face,
								t_mtl *mtl);

#endif
