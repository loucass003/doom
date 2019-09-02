/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 17:16:19 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/02 17:14:51 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include "obj.h"
# include "image.h"
# include "camera.h"
# include "polygon.h"
# include "maths/triangle.h"

typedef struct		s_triangle
{
	t_vertex		a;
	t_vertex		b;
	t_vertex		c;
}					t_triangle;

typedef enum		s_render_type
{
	CTX_NORMAL,
	CTX_SHADOW_MAPPING
}					t_render_type;

typedef struct		s_zbuff
{
	float			depth;
	uint8_t			light_intensity;
}					t_zbuff;

typedef struct		s_render_data
{
	t_triangle		triangle;
	t_mtl			*mtl;
}					t_render_data;

typedef struct		s_render_context
{
	t_render_type	type;
	t_camera		*camera;
	t_zbuff			*buffer;
	t_img			*image;
	t_polygon		*pointed_polygon;
	struct s_doom	*doom;
	// struct s_light	*lights;
}					t_render_context;


void				draw_triangle(t_render_context *ctx, t_render_data data);
void				process_triangle(t_render_context *ctx, t_mtl *mtl, t_triangle triangle);
void				post_process_triangle(t_render_context *ctx, t_mtl *mtl, t_triangle triangle);
void				render_polygon(t_render_context *ctx, t_polygon *poly);
void				render_obj(t_render_context *ctx, t_obj *obj);


#endif
