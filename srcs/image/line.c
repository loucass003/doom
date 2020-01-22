/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/02 00:28:50 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/22 11:10:39 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "image.h"
#include "render.h"

void	draw_line(t_img *img, t_pixel p0, t_pixel p1)
{
	t_pixel			d;
	t_pixel			s;
	int				e[2];

	d = (t_pixel){ ft_abs(p1.x - p0.x), ft_abs(p1.y - p0.y), 0 };
	s = (t_pixel){ (p0.x < p1.x ? 1 : -1), (p0.y < p1.y ? 1 : -1), 0 };
	e[0] = (d.x > d.y ? d.x : -d.y) / 2;
	while (p0.x != p1.x || p0.y != p1.y)
	{
		if (p0.x >= 0 && p0.x < (int)img->width 
			&& p0.y >= 0 && p0.y < (int)img->height)
			img->pixels[p0.y * img->width + p0.x] = p0.color;
		e[1] = e[0];
		if (e[1] > -d.x)
		{
			e[0] -= d.y;
			p0.x += s.x;
		}
		if (e[1] < d.y)
		{
			e[0] += d.x;
			p0.y += s.y;
		}
	}
}

void	ft_swap(float *a, float *b)
{
	float tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

// 	return (ft_vec3_add(a, ft_vec3_mul_s(ft_vec3_sub(b, a), alpha)));

void	draw_line_zbuff(t_render_context *ctx, t_vec4 v0, t_vec4 v1, int color)
{
	t_bool	steep;
	t_vec3	v;

	steep = FALSE;
	if (ft_abs(v0.x - v1.x) < ft_abs(v0.y - v1.y)) {
		ft_swap(&v0.x, &v0.y);
		ft_swap(&v1.x, &v1.y);
		steep = TRUE;
	}
	if (v0.x > v1.x) { 
        ft_swap(&v0.x, &v1.x); 
    	ft_swap(&v0.y, &v1.y); 
		ft_swap(&v0.w, &v1.w); 
	}
	int dx = v1.x-v0.x; 
    int dy = v1.y-v0.y; 
    int derror2 = ft_abs(dy) * 2; 
    int error2 = 0; 
   	v.y = v0.y;
	v.x = v0.x - 1;
	while (++v.x < v1.x)
	{
		t_pixel p = steep ? (t_pixel){ v.y, v.x, 0 } : (t_pixel){ v.x, v.y, 0 };
		float a = (v.x - v0.x) / (v1.x - v0.x);
		float z = v0.w + (v1.w - v0.w) * a;
		if (p.x >= 0 && p.x < (int)ctx->image->width
			&& p.y >= 0 && p.y < (int)ctx->image->height
			&& ctx->buffer[(int)p.y * ctx->image->width + (int)p.x] >= z)
		{
			ctx->buffer[(int)p.y * ctx->image->width + (int)p.x] = z;
            ctx->image->pixels[(int)p.y * ctx->image->width + (int)p.x] = color;
		}
		error2 += derror2; 
        if (error2 > dx)
		{ 
            v.y += (v1.y > v0.y ? 1 : -1); 
            error2 -= dx * 2; 
        } 
	}
}
