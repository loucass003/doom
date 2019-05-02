/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:26:42 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/01 22:43:51 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BSP_H
# define BSP_H

# include "doom.h"

typedef enum	s_side
{
	S_FRONT,
	S_BACK,
	S_COLINEAR,
	S_SPANNING
}				t_side;

typedef enum	s_polygon_type
{
	P_WALL,
	P_FLOOR
}				t_polygon_type;

typedef struct	s_line
{
	t_vec2	a;
	t_vec2	b;
	t_vec2	normal;
}				t_line;

typedef struct	s_polygon
{
	t_polygon_type	type;
	t_arraylist		*vertices;
	t_vec2			*proj_vertices;
}				t_polygon;

typedef enum	s_node_type
{
	N_NODE,
	N_LEAF
}				t_node_type;

typedef struct	s_node
{
	t_node_type		type;
	t_line			partition;
	struct s_node	*front;
	struct s_node	*back;
	t_arraylist		*polygons;
}				t_node;

t_bool		get_partition(t_arraylist *polygons, t_line *partition);
t_side		get_side_thin(t_line partition, t_vec2 v);
t_side		get_side_thick(t_line partition, t_vec2 v);
t_side		get_poly_side(t_line partition, t_polygon *poly);
float		get_intersection(t_line a, t_line b);

t_vec2		get_polygon_normal(t_polygon *poly);
t_line		get_poly_line(t_polygon *poly);
t_polygon	*clip_poly(t_polygon *poly, t_line partition, t_side side);

t_polygon	*create_wall_polygon(t_line	line, float floor, float ceil);
t_polygon	*create_polygon(t_arraylist *vertices, t_polygon_type type);

void		free_polygon(void *e);
void		free_node(t_node *n);

void		print_node(t_node *n);
t_node		*create_node(t_arraylist *polygons);

void		build_node(t_node *node);

#endif
