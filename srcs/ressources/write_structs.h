/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_structs.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 05:41:51 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/15 03:41:46 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRITE_STRUCTS_H
# define WRITE_STRUCTS_H

# include <libft.h>
# include "ressource.h"
# include "editor.h"
# include "render.h"

# pragma pack(push, 1)

typedef struct		s_wr_songs
{
	uint32_t	sample_rate;
	uint16_t	bits_per_sample;
	uint32_t	buffer_size;
}					t_wr_songs;

typedef struct		s_wr_ressource
{
	t_ressource_type	type;
	t_bool				fixed;
	t_bool				loaded;
	int					name_len;
}					t_wr_ressource;

typedef struct		s_wr_room
{
	t_bool			closed;
	int				walls_count;
	int				floor_res_index;
	int				ceiling_res_index;
	t_vec3			floor_rot;
	t_vec3			ceil_rot;
	t_bool			floor_invisible;
	t_bool			ceil_invisible;
	t_bool			floor_collision;
	t_bool			ceil_collision;
	uint8_t			floor_normal;
	uint8_t			ceil_normal;
	uint8_t			ambiant_light;
	t_vec2			floor_uv_repeat;
	t_vec2			floor_uv_offset;
	t_vec2			ceil_uv_repeat;
	t_vec2			ceil_uv_offset;
}					t_wr_room;

typedef struct		s_wr_sprite
{
	t_bool			always_facing_player;
	int				texture_index;
	t_vec3			hitbox_radius;
}					t_wr_sprite;

typedef struct		s_wr_object
{
	t_object_type	type;
	t_vec3			pos;
	t_vec3			scale;
	t_vec3			rotation;
	t_bool			no_light;
}					t_wr_object;

typedef struct		s_wr_wall
{
	int				indice;
	float			floor_height;
	float			ceiling_height;
	int				wall_sections_count;
}					t_wr_wall;

typedef struct		s_wr_wall_section
{
	t_wall_section_type	type;
	int					resource_index;
	uint8_t				normal_type;
	t_bool				invisible;
	t_bool				collisions;
	t_vec2				uv_repeat;
	t_vec2				uv_offset;
}					t_wr_wall_section;

typedef struct		s_wr_img
{
	size_t			size;
	uint32_t		width;
	uint32_t		height;
	t_bool			secure;
}					t_wr_img;

typedef struct		s_wr_model
{
	int				vertices_count;
	int				vertex_count;
	int				normals_count;
	int				materials_count;
	int				faces_count;
	int				groups_count;
	char			groups[GROUPS_MAX][GROUPS_NAME_LEN];
}					t_wr_model;

typedef struct		s_wr_mtl
{
	t_bool			texture_map_set;
	t_bool			material_color_set;
	t_bool			transparent;
	t_bool			wireframe;
	int				material_color;
}					t_wr_mtl;

typedef struct		s_wr_player
{
	t_bool			set;
	t_vec3			position;
	t_vec3			rotation;
}					t_wr_player;

typedef struct		s_wr_globals
{
	t_bool			skybox;
}					t_wr_globals;

typedef struct		s_wr_data
{
	t_wr_header		header;
}					t_wr_data;

# pragma pack(pop)

t_bool		read_model_arrays(t_ressource_manager *r, t_renderable *model,
	t_wr_model wr_model);
t_bool		alloc_model(t_renderable *model, t_wr_model wr_model);

#endif
