/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:26:42 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/10 15:59:04 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BSP_H
# define BSP_H

# include "doom.h"

typedef struct		s_vec4
{
	float			x;
	float			y;
	float			z;
	float			w;
}					t_vec4;

typedef	union		u_vec4_u
{
	float			a[4];
	t_vec4			v;
}					t_vec4_u;

typedef struct		s_ints
{
	int				len;
	int				capacity;
	int				values[];
}					t_ints;

typedef struct		s_4dvertices
{
	int				len;
	int				capacity;
	t_vec4			vertices[];
}					t_4dvertices;

typedef struct		s_3dvertices
{
	int				len;
	int				capacity;
	t_vec3			vertices[];
}					t_3dvertices;

typedef struct		s_2dvertices
{
	int				len;
	int				capacity;
	t_vec2			vertices[];
}					t_2dvertices;

typedef enum		s_polygon_type
{
	P_WALL,
	P_FLOOR,
	P_CEILING
}					t_polygon_type;

typedef struct		s_polygon
{
	t_polygon_type	type;
	t_3dvertices	*vertices;
	t_vec2			*uvs;
	t_ints			*indices;
	t_vec4			*pp_vertices;
	t_vec3			*normals;
}					t_polygon;

typedef struct		s_polygons
{
	int				len;
	int				capacity;
	t_polygon		polygons[];
}					t_polygons;

typedef enum		s_side
{
	S_FRONT = 1,
	S_BACK = -1,
	S_COLINEAR = 0,
	S_SPANNING = 2
}					t_side;

typedef struct		s_line
{
	t_vec2	a;
	t_vec2	b;
	t_vec2	normal;
}					t_line;

typedef enum		s_node_type
{
	N_NODE,
	N_LEAF
}					t_node_type;

typedef struct		s_node
{
	t_node_type		type;
	t_line			partition;
	t_vec2			display_pos;
	int				depth;
	struct s_node	*front;
	struct s_node	*back;
	struct s_node	*parent;
	t_polygons		*polygons;
}					t_node;

t_bool		get_partition(t_polygons *polygons, t_line *partition);
t_side		get_side_thin(t_line partition, t_vec2 v);
t_side		get_side_thick(t_line partition, t_vec2 v);
t_side		get_poly_side(t_line partition, t_polygon *poly);
float		get_intersection(t_line a, t_line b);

t_vec3		get_polygon_normal(t_polygon *poly);

t_line		get_poly_line(t_polygon *poly);
t_bool		clip_poly(t_polygon *out, t_polygon *poly, t_line partition, t_side side);

t_polygon	create_wall_polygon(t_line	line, float floor, float ceil);
t_polygon	create_polygon(t_3dvertices *vertices, t_polygon_type type);

void		free_node(t_node *n);
void		free_polygons(t_polygons *polygons);

void		print_node(t_node *n);
t_node		*create_node(t_polygons *polygons);

void		build_node(t_node *node);
t_bool		post_process_bsp(t_node *n, int x, int depth);

t_bool		triangulate_polygon(t_polygon *polygon);
void		uv_mapping(t_polygon *polygon);

#endif
