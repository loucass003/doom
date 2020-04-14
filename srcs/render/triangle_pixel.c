/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle_pixel.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 23:22:34 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/14 23:23:15 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "render.h"
#include "constants.h"

t_bool				texture_map_color(t_render_data data, t_color_r *c,
	t_pixel_data pd, t_vertex vert)
{
	float	w;
	int		x;
	int		y;

	w = 1. / vert.pos.w;
	vert.tex.x = (1.0f - pd.t) * pd.start.tex.x + pd.t * pd.end.tex.x;
	vert.tex.y = (1.0f - pd.t) * pd.start.tex.y + pd.t * pd.end.tex.y;
	x = (int)(ft_abs(((vert.tex.x > 0) - vert.tex.x * w)
		* (data.mtl->texture_map->width)))
		% (data.mtl->texture_map->width);
	y = (int)(ft_abs(((vert.tex.y > 0) - vert.tex.y * w)
		* (data.mtl->texture_map->height)))
		% (data.mtl->texture_map->height);
	x = vert.tex.x > 0 ? (data.mtl->texture_map->width - 1) - x : x;
	y = vert.tex.y < 0 ? (data.mtl->texture_map->height - 1) - y : y;
	c->color = data.mtl->texture_map->pixels[(y
		* data.mtl->texture_map->width) + x];
	if (c->argb.a == 0)
		return (FALSE);
	return (TRUE);
}

void				draw_pixel(t_render_data data, t_pixel_data pd)
{
	t_vertex	vert;
	float		*buff;
	float		a;
	t_color_r	c;

	vert.pos.w = (1.0f - pd.t) * pd.start.pos.w + pd.t * pd.end.pos.w;
	buff = data.ctx->buffer + pd.y * (int)S_WIDTH + pd.x;
	if (vert.pos.w <= *buff)
	{
		a = ft_max(AMBIANT_LIGHT, (1.0f - pd.t) * pd.start.light_color
			+ pd.t * pd.end.light_color) / 255.0;
		if (data.mtl->material_color_set)
			c.color = data.mtl->material_color;
		if (data.mtl->texture_map_set && !texture_map_color(data, &c, pd, vert))
			return ;
		if (!data.mtl->transparent)
		{
			c.argb.r *= a;
			c.argb.g *= a;
			c.argb.b *= a;
		}
		data.ctx->image->pixels[pd.y * (int)S_WIDTH + pd.x] = c.color;
		*buff = vert.pos.w;
	}
}
