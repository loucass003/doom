/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 01:04:42 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/16 21:29:31 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLYGON_H
# define POLYGON_H

# include <libft.h>
# include <SDL.h>
# include "maths/vec4.h"
# include "arrays.h"
# include "maths/line.h"
# include "collision.h"
# include "render.h"


typedef enum		s_polygon_type
{
	P_WALL,
	P_FLOOR,
	P_CEILING
}					t_polygon_type;

typedef struct		s_polygon
{
	t_polygon_type	type;
}					t_polygon;

typedef struct		s_polygons
{
	int				len;
	int				capacity;
	t_polygon		polygons[];
}					t_polygons;

t_polygons			*create_polygons_array(int capacity);
t_polygons			*append_polygons_array(t_polygons **arr, t_polygon v);
t_polygons			*splice_polygons_array(t_polygons *arr,
						int index, int n);
t_polygons			*copy_polygons_array(t_polygons *src,
						t_polygons **dst);

t_vec3				get_polygon_normal(t_polygon *poly);

t_bool				create_wall_polygon(t_renderable *r, t_mtl mtl, t_line line, t_vec2 bounds);
t_bool				create_polygon(t_renderable *r, t_polygon_type type, t_mtl mtl);
t_bool				add_polygon_vertice(t_renderable *r, t_vec4 v);

t_bool				triangulate_polygon(t_renderable *r);
void				uv_mapping(t_4dvertices *vertices, t_2dvertices *vertex);
t_bool				post_process_polygon(struct s_doom *doom, t_renderable *r);

#endif
