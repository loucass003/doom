/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gltf.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/18 11:49:41 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/19 18:47:31 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLTF_H
# define GLTF_H
# include "doom.h"

typedef enum	e_alpha_mode
{
	alpha_opaque = 0,
	alpha_mask = 1,
	alpha_blend = 2
}				t_alpha_mode;

typedef enum	e_gltf_type
{
	gltf_type_scalar = 0,
	gltf_type_vec2 = 1,
	gltf_type_vec3 = 2,
	gltf_type_vec4 = 3,
	gltf_type_mat2 = 4,
	gltf_type_mat3 = 5,
	gltf_type_mat4 = 6
}				t_gltf_type;

typedef enum	e_primitive_mode
{
	mode_points = 0,
	mode_lines = 1,
	mode_line_loop = 2,
	mode_line_strip = 3,
	mode_triangles = 4,
	mode_triangle_strip = 5,
	mode_triangle_fan = 6
}				t_primitive_mode;

typedef	struct	s_gltf_node
{
	double		*childrens;
	size_t		childrens_count;
	t_vec3		translation;
	t_vec4		rotation;
	t_vec3		scale;
	t_bool		matrix_set;
	t_mat4		matrix;
	t_bool		mesh_set;
	size_t		mesh;
	t_bool		camera_set;
	size_t		camera;
	char		*name;
	size_t		name_len;
}				t_gltf_node;

typedef	struct	s_scene
{
	char		*name;
	double		*nodes;
	size_t		nodes_count;
}				t_scene;

typedef struct	s_attributes
{
	t_bool		position_set;
	t_vec3		position;
	t_bool		normal_set;
	t_vec3		normal;
	t_bool		tangent_set;
	t_vec4		tangent;
	t_bool		texcoord_set;
	t_vec2		texcoord[2];
	t_bool		color_set;
	t_vec4		color[2];
	t_bool		joints_set;
	t_vec4		joints[2];
	t_bool		weights_set;
	t_vec4		weights[2];
}				t_attributes;

typedef struct	s_target
{
	t_vec3		position;
	t_vec3		normal;
	t_vec3		tangent;
}				t_target;

typedef	struct	s_primitive
{
	size_t				indices;
	t_bool				material_set;
	size_t				material;
	t_primitive_mode	mode;
	t_attributes		attributes;
	t_target			*targets;
	size_t				targets_count;
}				t_primitive;

typedef struct	s_mesh
{
	char		*name;
	size_t		name_len;
	t_primitive	*primitives;
	size_t		primitives_count;
	double		*weights;
	size_t		weights_count;
}				t_mesh;

typedef struct	s_buffer
{
	char		*uri;
	size_t		uri_len;
	size_t		byte_len;
	char		*name;
	size_t		name_len;
}				t_buffer;

typedef struct	s_buffer_view
{
	size_t		buffer;
	size_t		byte_offset;
	size_t		byte_len;
	size_t		byte_stride;
	size_t		target;
	char		*name;
	size_t		name_len;
}				t_buffer_view;

typedef	struct	s_gltf_indices
{
	size_t		buffer_view;
	size_t		buffer_offset;
	size_t		component_type;
} 				t_gltf_indices;

typedef struct	s_gltf_values
{
	size_t		buffer_view;
	size_t		buffer_offset;
}				t_gltf_values;

typedef struct	s_sparse
{
	size_t			count;
	t_gltf_indices	indices;
	t_gltf_values	values;
}				t_sparse;

typedef struct	s_accessor
{
	size_t		buffer_view;
	size_t		byte_offset;
	size_t		component_type;
	t_bool		normalized_set;
	t_bool		normalized;
	size_t		count;
	t_gltf_type	type;
	t_bool		max_set;
	double		max;
	t_bool		min_set;
	double		min;
	t_bool		sparse_set;
	t_sparse	sparse;
	char		*name;
	size_t		name_len;
}				t_accessor;

/*
** WARNING: MISS DATA
*/
typedef struct	s_gltf_pbrmetallic_roughness 
{
	t_vec4		base_color_factor;
	double		metallic_factor;
	double		roughness_factor;
}				t_gltf_pbrmetallic_roughness;

typedef struct	s_occlusion_texture_info
{
	size_t		index;
	size_t		text_coord;
	double		strength;
}				t_occlusion_texture_info;

typedef struct	s_normal_texture_info
{
	size_t		index;
	size_t		text_coord;
	double		scale;
}				t_normal_texture_info;

typedef struct	s_material
{
	char							*name;
	size_t							name_len;
	t_vec3							emissive_factor;
	t_alpha_mode					alpha_mode;
	double							alpha_cutoff;
	t_bool							double_sided;
	t_bool							pbrmetallic_roughness_set;
	t_gltf_pbrmetallic_roughness	pbrmetallic_roughness;
	t_bool							occlusion_texture_set;
	t_occlusion_texture_info		occlusion_texture;
	t_bool							normal_texture_set;
	t_normal_texture_info			normal_texture;
}				t_material;

typedef struct	s_gltf
{
	t_scene			*scenes;
	size_t			scenes_count;
	t_gltf_node		*nodes;
	size_t			nodes_count;
	t_mesh			*meshes;
	size_t			meshes_count;
	t_buffer		*buffers;
	size_t			buffers_count;
	t_buffer_view	*buffer_views;
	size_t			buffer_views_count;
	t_accessor		*accessors;
	size_t			accessors_count;
	t_material		*materials;
	size_t			materials_count;
}				t_gltf;

#endif
