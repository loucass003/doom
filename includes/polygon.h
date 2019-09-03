/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 01:04:42 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/03 16:32:07 by llelievr         ###   ########.fr       */
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
	t_collidable	*collidables;
	SDL_Surface		*texture;
	uint8_t			*lightmap;
	t_bool			transparent;
	t_mat4			matrix;
	t_vec3			*normals;
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

t_polygon			create_wall_polygon(SDL_Surface *texture, t_line line, float floor, float ceil);
t_polygon			create_polygon(t_3dvertices *vertices, t_polygon_type type, SDL_Surface *texture);

t_bool				triangulate_polygon(t_polygon *polygon);
void				uv_mapping(t_polygon *polygon);
t_bool				post_process_polygon(t_polygon *poly);

#endif
